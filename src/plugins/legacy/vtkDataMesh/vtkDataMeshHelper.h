#pragma once

// medInria
#include <medAbstractData.h>
#include <medMetaDataKeys.h>
#include <vtkMetaDataSet.h>

// vtk
#include <vtkAbstractArray.h>
#include <vtkFieldData.h>

namespace DataMeshHelper
{
    /**
     * @brief This function is called when a metadata needs to be written
     * or read. There is a bug concerning NaNs in the VTK ASCII reader/writer:
     * this function replaces all NaN into numeric_limits<double>::lowest when
     * writting, and vice-versa when reading.
     *
     * @param metaData
     * @param write true if data must be writte, false when data is read
     */
    void prepareMetaDataForAsciiReadOrWrite(vtkMetaDataSet* metaData, bool write);

    /**
     * @brief Iterates on all arrays in a vtkFieldData and calls
     * replaceNaNsFromArray(...)
     *
     * @param fieldData
     * @param write
     */
    void replaceNaNsFromFieldData(vtkFieldData* fieldData, bool write);

    /**
     * @brief When writting a data, replaces all NaN with
     * numeric_limits<double>::lowest. When reading, does the opposite.
     *
     * @param array
     * @param write
     */
    void replaceNaNsFromArray(vtkDataArray* array, bool write);
}

