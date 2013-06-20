// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////
#include "itkDataSHImageReaderPlugin.h"

#include <dtkLog/dtkLog.h>

#include <dtkItkLogForwarder/itkLogForwarder.h>

#include "itkNrrdDataSHImageReader.h"
#include "itkNiftiDataSHImageReader.h"
#include "itkMetaDataSHImageReader.h"

// /////////////////////////////////////////////////////////////////
// itkDataSHImageReaderPluginPrivate
// /////////////////////////////////////////////////////////////////

class itkDataSHImageReaderPluginPrivate
{
public:
  itkDataSHImageReaderPluginPrivate()
  {
      forwarder = itk::LogForwarder::New();
  }
  ~itkDataSHImageReaderPluginPrivate()
  {

  }
  // Class variables go here.
  itk::LogForwarder::Pointer forwarder;
};

// /////////////////////////////////////////////////////////////////
// itkDataSHImageReaderPlugin
// /////////////////////////////////////////////////////////////////

itkDataSHImageReaderPlugin::itkDataSHImageReaderPlugin(QObject *parent) : dtkPlugin(parent), d(new itkDataSHImageReaderPluginPrivate)
{

}

itkDataSHImageReaderPlugin::~itkDataSHImageReaderPlugin(void)
{
    delete d;

    d = NULL;
}

bool itkDataSHImageReaderPlugin::initialize(void)
{
    if(!itkMetaDataSHImageReader::registered())  dtkWarn() << "Unable to register itkMetaDataSHImageReader type";
    if(!itkNiftiDataSHImageReader::registered()) dtkWarn() << "Unable to register itkNiftiDataSHImageReader type";
    if(!itkNrrdDataSHImageReader::registered())  dtkWarn() << "Unable to register itkNrrdDataSHImageReader type";

    return true;
}

bool itkDataSHImageReaderPlugin::uninitialize(void)
{
    return true;
}

QString itkDataSHImageReaderPlugin::name(void) const
{
    return "itkDataSHImageReaderPlugin";
}

QString itkDataSHImageReaderPlugin::description(void) const
{
    return "ITK SH data image readers";
}

QStringList itkDataSHImageReaderPlugin::tags(void) const
{
    return QStringList() << "itk" << "data" << "SH" << "image" << "reader";
}

QStringList itkDataSHImageReaderPlugin::types(void) const
{
    return QStringList() << "itkMetaDataSHImageReader"
                         << "itkNiftiDataSHImageReader"
                         << "itkNrrdDataSHImageReader";
}

Q_EXPORT_PLUGIN2(itkDataSHImageReaderPlugin, itkDataSHImageReaderPlugin)
