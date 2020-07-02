/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "vtkDataMesh4DInteractor.h"

#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkMetaDataSetSequence.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

#include <medAbstractImageView.h>
#include <medVtkViewBackend.h>
#include <medViewFactory.h>
#include <medTimeLineParameterL.h>

class vtkDataMesh4DInteractorPrivate
{
public:
    typedef vtkSmartPointer <vtkActor>  ActorSmartPointer;
    typedef vtkSmartPointer <vtkProperty>  PropertySmartPointer;

    medAbstractData *data;
    medAbstractImageView *view;
    vtkImageView2D *view2d;
    vtkImageView3D *view3d;

    vtkMetaDataSetSequence *sequence;

    vtkSmartPointer<vtkTextActor> textActor;
};


vtkDataMesh4DInteractor::vtkDataMesh4DInteractor(medAbstractView* parent): vtkDataMeshInteractor(parent),
    d(new vtkDataMesh4DInteractorPrivate)
{
    d->view = dynamic_cast<medAbstractImageView *>(parent);
    d->data = nullptr;

    medVtkViewBackend* backend = static_cast<medVtkViewBackend*>(parent->backend());
    d->view2d = backend->view2D;
    d->view3d = backend->view3D;

    d->textActor = nullptr;
}

vtkDataMesh4DInteractor::~vtkDataMesh4DInteractor()
{

}

QString vtkDataMesh4DInteractor::description() const
{
    return tr("Interactor displaying 4D Meshes");
}

QString vtkDataMesh4DInteractor::identifier() const
{
    return "vtkDataMesh4DInteractor";
}

QStringList vtkDataMesh4DInteractor::handled() const
{
    return vtkDataMesh4DInteractor::dataHandled();
}

QStringList vtkDataMesh4DInteractor::dataHandled()
{
    QStringList d = QStringList() << "vtkDataMesh4D";
    return  d;
}

bool vtkDataMesh4DInteractor::registered()
{
    medViewFactory *factory = medViewFactory::instance();
    return factory->registerInteractor<vtkDataMesh4DInteractor>("vtkDataMesh4DInteractor",
                                                                          QStringList () << "medVtkView" <<
                                                                          vtkDataMesh4DInteractor::dataHandled());
}

void vtkDataMesh4DInteractor::setInputData(medAbstractData *data)
{
    vtkDataMeshInteractor::setInputData(data);

    if (data->identifier() == "vtkDataMesh4D" )
    {
        d->data = data;

        vtkMetaDataSetSequence *sequence = dynamic_cast<vtkMetaDataSetSequence *>((vtkDataObject *)(data->data()));
        //vtkProperty *prop = vtkProperty::New();
        if ( !sequence )
            return;

        switch (sequence->GetType())
        {
        case vtkMetaDataSet::VTK_META_SURFACE_MESH:
        case vtkMetaDataSet::VTK_META_VOLUME_MESH:
            d->sequence = sequence;
            d->data->setMetaData("SequenceDuration", QString::number(d->sequence->GetMaxTime()));
            d->data->setMetaData("SequenceFrameRate", QString::number((double)d->sequence->GetNumberOfMetaDataSets() /
                                                                           (double)d->sequence->GetMaxTime()));
            break;
        default:
            break;
        }
    }
}

void vtkDataMesh4DInteractor::setCurrentTime (double time)
{
    if(d->sequence->GetTime() == time)
        return;

    d->sequence->UpdateToTime(time);

    // Set the current time on the view if needed
    QString displayedTime = d->view->timeLineParameter()->getDisplayedTime();
    if (displayedTime != "")
    {
        // Refresh view size in case of resize
        QSize size = d->view->viewWidget()->size();
        int newSizeX = (int)((double)size.width()/90.0);
        int newSizeY = (int)((double)size.height()/1.5);

        // Display Time + Shift
        if (d->textActor == nullptr)
        {
            d->textActor = vtkSmartPointer<vtkTextActor>::New();
            d->textActor->SetInput(displayedTime.toStdString().c_str());
            d->textActor->SetDisplayPosition(newSizeX, newSizeY);
            d->textActor->GetTextProperty()->SetColor(1.0, 0.0, 0.0);
            d->textActor->GetTextProperty()->SetFontSize(20);

            d->view3d->GetRenderer()->AddViewProp(d->textActor);
            d->view2d->GetRenderer()->AddViewProp(d->textActor);
         }
        else
        {
            d->textActor->SetInput(displayedTime.toStdString().c_str());
            d->textActor->SetDisplayPosition(newSizeX, newSizeY);
        }
        d->view->render();
    }
    else
    {
        if (d->textActor != nullptr)
        {
            d->view3d->GetRenderer()->RemoveViewProp(d->textActor);
            d->view2d->GetRenderer()->RemoveViewProp(d->textActor);
            d->textActor = nullptr;
        }
    }
}

