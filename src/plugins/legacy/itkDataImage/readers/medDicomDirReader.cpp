/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medDicomDirReader.h"

const char medDicomDirReader::ID[] = "medDicomDirReader";

// /////////////////////////////////////////////////////////////////
// medDicomDirReaderPrivate
// /////////////////////////////////////////////////////////////////

class medDicomDirReaderPrivate
{
public:
    DcmFileFormat fileformat;

    QString dicomdirPath;        // ex: /home/toto/dicoms/DICOMDIR
    QString dicomsDirectoryPath; // ex: /home/toto/dicoms/DICOM
    std::vector<std::string> filelist;
};

// /////////////////////////////////////////////////////////////////
// medDicomDirReader
// /////////////////////////////////////////////////////////////////

medDicomDirReader::medDicomDirReader() : dtkAbstractDataReader(), d(new medDicomDirReaderPrivate)
{
    d->fileformat = nullptr;
}

medDicomDirReader::~medDicomDirReader()
{
    delete d;
    d = nullptr;
}

bool medDicomDirReader::registered()
{
    return medAbstractDataFactory::instance()->registerDataReaderType(
        "medDicomDirReader",
        QStringList()
            << "itkDataImageDouble3"
            << "itkDataImageFloat3"
            << "itkDataImageULong3"
            << "itkDataImageLong3"
            << "itkDataImageUInt3"
            << "itkDataImageInt3"
            << "itkDataImageInt4"
            << "itkDataImageLong4"
            << "itkDataImageUInt4"
            << "itkDataImageULong4"
            << "itkDataImageUShort3"
            << "itkDataImageUShort4"
            << "itkDataImageShort3"
            << "itkDataImageShort4"
            << "itkDataImageUChar3"
            << "itkDataImageUChar4"
            << "itkDataImageChar3"
            << "itkDataImageChar4"
            << "itkDataImageRGB3",
        createmedDicomDirReader);
}

QStringList medDicomDirReader::handled() const
{
    return QStringList() << "itkDataImageDouble3"
                         << "itkDataImageFloat3"
                         << "itkDataImageULong3"
                         << "itkDataImageLong3"
                         << "itkDataImageUInt3"
                         << "itkDataImageInt3"
                         << "itkDataImageInt4"
                         << "itkDataImageUInt4"
                         << "itkDataImageULong4"
                         << "itkDataImageLong4"
                         << "itkDataImageUShort3"
                         << "itkDataImageUShort4"
                         << "itkDataImageShort3"
                         << "itkDataImageShort4"
                         << "itkDataImageUChar3"
                         << "itkDataImageUChar4"
                         << "itkDataImageChar3"
                         << "itkDataImageChar4"
                         << "itkDataImageRGB3";
}

QString medDicomDirReader::identifier() const
{
    return ID;
}

QString medDicomDirReader::description() const
{
    return "Reader of DICOMDIR images with DCMTK, ITK::GDCM and VTK";
}

bool medDicomDirReader::canRead(const QString &path)
{
    bool res = false;
    if (path.endsWith("DICOMDIR", Qt::CaseInsensitive))
    {
        OFCondition status = d->fileformat.loadFile(path.toStdString());
        if (status.good())
        {
            res = true;
        }
    }
    return res;
}

bool medDicomDirReader::canRead(const QStringList &paths)
{
    for (auto path : paths)
    {
        if (!this->canRead(path))
        {
            return false;
        }
    }
    return true;
}

bool medDicomDirReader::readInformation(const QString &path)
{
    QStringList paths;
    paths << path;
    return readInformation(paths);
}

bool medDicomDirReader::readInformation(const QStringList &paths)
{
    qDebug() << "### medDicomDirReader::readInformation" << paths[0];
    // todo for each paths?

    if (paths.size() == 0)
    {
        return false;
    }

    saveNeededPaths(paths[0]);
    auto pixelType = GetPixelType();
    
    // Create the shell of the new medAbstractData
    medAbstractData *medData = dynamic_cast<medAbstractData *>(this->data());
    if (!medData)
    {
        medData = medAbstractDataFactory::instance()->create(pixelType);
        this->setData(medData);
    }

    // Collect metadata from the DICOMDIR
    DcmMetaInfo *metaInfo = d->fileformat.getMetaInfo();
    DcmDataset *dataset = d->fileformat.getDataset();
    if (medData && metaInfo)
    {
        OFString version;
        if (metaInfo->findAndGetOFString(DCM_MediaStorageSOPClassUID, version).good())
        {
            std::cout << "@@ DICOM version: " << version << std::endl;
        }

        OFString studyInstanceUID;
        if (metaInfo->findAndGetOFString(DCM_StudyInstanceUID, studyInstanceUID).good())
        {
            std::cout << "@@ Study UID: " << studyInstanceUID << std::endl;
            medData->setMetaData(medMetaDataKeys::StudyInstanceUID.key(), studyInstanceUID.c_str());
        }

        OFString sopInstanceUID;
        if (metaInfo->findAndGetOFString(DCM_SOPInstanceUID, sopInstanceUID).good())
        {
            std::cout << "@@ Instance UID: " << sopInstanceUID << std::endl;
            medData->setMetaData(medMetaDataKeys::SOPInstanceUID.key(), sopInstanceUID.c_str());
        }

        OFString fileCreationDate;
        if (metaInfo->findAndGetOFString(DCM_FileMetaInformationGroupLength, fileCreationDate).good())
        {
            std::cout << "@@ File creation date: " << fileCreationDate << std::endl;
            medData->setMetaData(medMetaDataKeys::ImportationDate.key(), fileCreationDate.c_str());
        }

        OFString manufacturer;
        if (metaInfo->findAndGetOFString(DCM_Manufacturer, manufacturer).good())
        {
            std::cout << "@@ DICOM machine manufacturer: " << manufacturer << std::endl;
            medData->setMetaData(medMetaDataKeys::Manufacturer.key(), manufacturer.c_str());
        }
    }

    if (medData && dataset)
    {
        // Check if the file is a DICOMDIR
        if (dataset->tagExistsWithValue(DCM_DirectoryRecordSequence))
        {
            DcmStack stack;
            if (dataset->search(DCM_DirectoryRecordSequence, stack, ESM_fromHere, OFTrue).good())
            {
                DcmObject *dobj = stack.top();
                if (dobj->ident() == EVR_SQ)
                {
                    DcmSequenceOfItems *sequence = dynamic_cast<DcmSequenceOfItems *>(dobj);
                    if (sequence)
                    {
                        // Check records in directory
                        for (unsigned long i = 0; i < sequence->card(); ++i)
                        {
                            DcmItem *item = sequence->getItem(i);
                            if (item)
                            {
                                OFString patientName;
                                if (item->findAndGetOFString(DCM_PatientName, patientName).good())
                                {
                                    std::cout << "@@ Nom du patient : " << patientName << std::endl;
                                    medData->setMetaData(medMetaDataKeys::PatientName.key(), patientName.c_str());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // todo
    // DcmTagKey key = DCM_PatientName;
    // DCM_PatientName findAndGetOFString OFString
    // PatientID
    // PatientBirthDate
    // PatientSex
    // StudyDate
    // StudyTime
    // EchoTime
    // ImageOrientationPatient
    // ImageType
    // DCM_PixelRepresentation findAndGetUint16 Uint16 -> pas
    // DCM_BitsAllocated findAndGetUint16 Uint16 (8 or 16) -> pas

    return true;
}

/**
 * @brief From the DICOMDIR path, get DICOMS paths
 * 
 * @param dicomdirPath 
 */
void medDicomDirReader::saveNeededPaths(QString dicomdirPath)
{
    d->dicomdirPath = dicomdirPath;

    DcmDataset *dataset = d->fileformat.getDataset();
    if (dataset)
    {
        DcmSequenceOfItems *seq = nullptr;
        if (dataset->findAndGetSequence(DCM_DirectoryRecordSequence, seq).good() && seq)
        {
            for (unsigned long i = 0; i < seq->card(); ++i)
            {
                DcmItem *item = seq->getItem(i);
                if (item)
                {
                    // Get the name of DICOMs directory
                    OFString referencedFileID;
                    if (item->findAndGetOFString(DCM_ReferencedFileID, referencedFileID).good())
                    {
                        getDicomsDirectoryPath(referencedFileID);
                        getFilenamesList();
                        break;
                    }
                }
            }
        }
    }
}

/**
 * @brief Compute the path of DICOMs directory
 * 
 * @param dicomsDirectoryName 
 */
void medDicomDirReader::getDicomsDirectoryPath(OFString dicomsDirectoryName)
{
    QFileInfo fileInfo(d->dicomdirPath);
    d->dicomsDirectoryPath = fileInfo.path() + QDir::separator() + dicomsDirectoryName.c_str();
}

/**
 * @brief Get the list of filenames in DICOM directory recursively
 * 
 */
void medDicomDirReader::getFilenamesList()
{
    auto fileNamesListGenerator = itk::GDCMSeriesFileNames::New();
    fileNamesListGenerator->SetRecursive(true);
    fileNamesListGenerator->SetDirectory(d->dicomsDirectoryPath.toUtf8().constData());
    d->filelist = fileNamesListGenerator->GetInputFileNames();
}

QString medDicomDirReader::GetPixelType()
{
    //todo: 4d
    QString pixelType = "itkDataImageFloat3";

    // VTK allows to get back directly the pixel type. 
    // An other solution would be to use "DcmFileFormat file; auto loadStatus = file.loadFile(d->filelist[0])"
    // to open a specific DICOM, then to access its dataset, and DCM_BitsAllocated and DCM_PixelRepresentation values.
    // These values could be used to compute the pixel data.
    // However in some DICOMDIR unsigned or signed type seems to be weirdly set, so it's better to use VTK for that.
    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetFileName(d->filelist[0].c_str());
    reader->Update();

    vtkImageData* imageData = reader->GetOutput();
    int scalarType = imageData->GetScalarType();

    switch (scalarType)
    {
        case VTK_UNSIGNED_CHAR:
            pixelType = "itkDataImageUChar3";
            break;
        case VTK_CHAR:
            pixelType = "itkDataImageChar3";
            break;
        case VTK_UNSIGNED_SHORT:
            pixelType = "itkDataImageUShort3";
            break;
        case VTK_SHORT:
            pixelType = "itkDataImageShort3";
            break;
        case VTK_UNSIGNED_INT:
            pixelType = "itkDataImageUInt3";
            break;
        case VTK_INT:
            pixelType = "itkDataImageInt3";
            break;
        case VTK_FLOAT:
            pixelType = "itkDataImageFloat3";
            break;
        case VTK_DOUBLE:
            pixelType = "itkDataImageDouble3";
            break;
        default:
            std::cout << "DICOMDIR: unhandled data type" << std::endl;
            break;
    }

    return pixelType;
}

bool medDicomDirReader::read(const QString &path)
{
    QStringList paths;
    paths << path;
    return read(paths);
}

bool medDicomDirReader::read(const QStringList &paths)
{
    qDebug() << "### medDicomDirReader::read " << paths[0];
    bool res = false;

    if (paths.size() == 0)
    {
        return res;
    }

    // todo plusieurs paths?

    if (readInformation(paths))
    {
        res = loadDICOMImagesFromDirectory(d->dicomsDirectoryPath);
    }

    return res;
}

bool medDicomDirReader::loadDICOMImagesFromDirectory(const QString &dicomDirPath)
{
    qDebug() << "### medDicomDirReader::loadDICOMImagesFromDirectory << " << dicomDirPath;
    bool res = true;

    if (medAbstractData *medData = dynamic_cast<medAbstractData *>(this->data()))
    {
        auto io = itk::GDCMImageIO::New();
        try
        {
            if (medData->identifier() == "itkDataImageUChar3")
            {
                read3DImage<unsigned char>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageChar3")
            {
                read3DImage<char>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageUShort3")
            {
                read3DImage<unsigned short>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageShort3")
            {
                read3DImage<short>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageUInt3")
            {
                read3DImage<unsigned int>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageInt3")
            {
                read3DImage<int>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageULong3")
            {
                read3DImage<unsigned long>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageLong3")
            {
                read3DImage<long>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageFloat3")
            {
                read3DImage<float>(medData, io, d->filelist);
            }
            else if (medData->identifier() == "itkDataImageDouble3")
            {
                read3DImage<double>(medData, io, d->filelist);
            }
            // else if (medData->identifier()=="itkDataImageUChar4")  { read4DImage<unsigned char>(medData, io,map);       }
            // else if (medData->identifier()=="itkDataImageUShort4") { read4DImage<unsigned short>(medData, io,map);      }
            // else if (medData->identifier()=="itkDataImageShort4")  { read4DImage<short>(medData, io,map);               }
            // else if (medData->identifier()=="itkDataImageUInt4")   { read4DImage<unsigned int>(medData, io,map);        }
            // else if (medData->identifier()=="itkDataImageULong4")  { read4DImage<unsigned long>(medData, io,map);       }
            // else if (medData->identifier()=="itkDataImageInt4")    { read4DImage<int>(medData, io,map);                 }
            // else if (medData->identifier()=="itkDataImageLong4")   { read4DImage<long>(medData, io,map);                }
            // else if (medData->identifier()=="itkDataImageChar4")   { read4DImage<char>(medData, io,map);                }
            // else if (medData->identifier()=="itkDataImageDouble4") {
            /**
            @todo Handle properly double pixel values.
            For the moment it is only handled in 3D, not in 4D, and it is very
            common to have 4D double images (cardiac).
            This hack just downcast images in short when the dimension is 4.
            which is WRONG.
            */
            // read4DImage<short>(medData,d->io,map);
            //}
            else
            {
                qDebug() << "DICOMDIR: unhandled medData type : " << medData->identifier();
                res = false;
            }
        }
        catch (itk::ExceptionObject &e)
        {
            qDebug() << "DICOMDIR: " << e.GetDescription();
            res = false;
        }
    }
    return res;
}

template <typename TYPE>
void medDicomDirReader::read3DImage(medAbstractData *medData,
                                         itk::GDCMImageIO::Pointer io,
                                         std::vector<std::string> filelist)
{
    auto reader = itk::ImageSeriesReader<itk::Image<TYPE, 3>>::New();
    reader->SetImageIO(io);
    reader->SetFileNames(filelist);
    medData->setData(reader->GetOutput());
    reader->Update();
}

void medDicomDirReader::setProgress(int value)
{
    emit progressed(value);
}

// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractDataReader *createmedDicomDirReader()
{
    return new medDicomDirReader;
}
