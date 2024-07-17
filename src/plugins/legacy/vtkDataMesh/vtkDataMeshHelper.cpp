#include "vtkDataMeshHelper.h"

// vtk
#include <vtkArrayDispatch.h>
#include <vtkAssume.h>
#include <vtkCellData.h>
#include <vtkDataArrayAccessor.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkStringArray.h>

namespace DataMeshHelper
{

// Modern implementation of manipulating arrays using new concepts in VTK 7.1:
struct ModifyArrayForWriteOrRead
{
    template <typename VectorArray>
    void operator()(VectorArray* array)
    {
        VTK_ASSUME(array->GetNumberOfComponents() == 1);

        vtkDataArrayAccessor<VectorArray> v(array);
        // It seems that taking the lowest possible value
        // (std::numeric_limits<double>::lowest) causes a bug.
        // Here we instead take a value that will hopefully never be used...
        double replacementValue = -1E10;
        vtkIdType nbTuples = array->GetNumberOfTuples();
        for (vtkIdType tupleIdx = 0; tupleIdx < nbTuples; ++tupleIdx)
        {
            if (write && vtkMath::IsNan(v.Get(tupleIdx, 0)))
            {
                v.Set(tupleIdx, 0, replacementValue);
            }
            else if (!write && v.Get(tupleIdx, 0) == replacementValue)
            {
                v.Set(tupleIdx, 0, vtkMath::Nan());
            }
        }
    }

    bool write = true;
};

void prepareMetaDataForAsciiReadOrWrite(vtkMetaDataSet* metaData, bool replaceNaNs)
{
    vtkDataSet* dataSet = metaData->GetDataSet();
    if (dataSet)
    {
        replaceNaNsFromFieldData(dataSet->GetPointData(), replaceNaNs);
        replaceNaNsFromFieldData(dataSet->GetCellData(), replaceNaNs);
        replaceNaNsFromFieldData(dataSet->GetFieldData(), replaceNaNs);
    }
}

void replaceNaNsFromFieldData(vtkFieldData* fieldData, bool replaceNaNs)
{
    for (int i = 0; i < fieldData->GetNumberOfArrays(); ++i)
    {
        vtkAbstractArray* array = fieldData->GetAbstractArray(i);
        // restrain to double arrays because the NaN value is coded as double
        // so it cannot be found in other array types
        if (array && array->GetDataType() == VTK_DOUBLE && array->GetNumberOfComponents() == 1)
        {
            replaceNaNsFromArray(vtkDataArray::SafeDownCast(array), replaceNaNs);
        }
    }
}

void replaceNaNsFromArray(vtkDataArray* abstractArray, bool replaceNaNs)
{
    ModifyArrayForWriteOrRead worker;
    worker.write = replaceNaNs;
    typedef vtkArrayDispatch::DispatchByValueType<vtkArrayDispatch::Reals> Dispatcher;

    if (!Dispatcher::Execute(abstractArray, worker))
    {
        // fallback
        worker(abstractArray);
    }
}

}
