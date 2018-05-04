/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medReformatWorkspace.h>

#include <dtkCoreSupport/dtkSignalBlocker.h>

#include <medViewFactory.h>
#include <medAbstractView.h>

#include <medReformatSelectorToolBox.h>
#include <medViewContainer.h>
#include <medTabbedViewContainers.h>
#include <medReformatSelectorToolBox.h>
#include <medAbstractLayeredView.h>
#include <medStringListParameterL.h>

#include <medToolBoxFactory.h>

#include <medViewParameterGroupL.h>
#include <medLayerParameterGroupL.h>
#include <medViewEventFilter.h>
#include <medViewContainerManager.h>

class medReformatWorkspacePrivate
{
public:
    medReformatSelectorToolBox * reformatToolBox;
    medViewContainer *fixedContainer;
};

medReformatWorkspace::medReformatWorkspace(QWidget *parent)
    : medAbstractWorkspaceLegacy(parent), d(new medReformatWorkspacePrivate)
{
    d->reformatToolBox = new medReformatSelectorToolBox(parent);

    //this->setUserLayerPoolable(false);
    //connect(this->tabbedViewContainers(), SIGNAL(currentChanged(int)), this, SLOT(updateUserLayerClosable(int)));
    //connect(d->reformatToolBox, SIGNAL(movingDataRegistered(medAbstractData*)), this, SLOT(updateFromReformatSuccess(medAbstractData*)));
    //connect(d->reformatToolBox, SIGNAL(destroyed()), this, SLOT(removeSelectorInternToolBox()));
    //connect(d->fixedContainer, SIGNAL(viewContentChanged()), this, SLOT(updateFromFixedContainer()));

//    connect(d->reformatToolBox, SIGNAL(installEventFilterRequest(medViewEventFilter*)),
//            this, SLOT(addViewEventFilter(medViewEventFilter*)));

    //connect(d->segmentationToolBox,SIGNAL(success()),this,SLOT(onSuccess()));

    // Always have a parent.
    if (!parent)
    {
        throw (std::runtime_error ("Must have a parent widget"));
    }

    // Adds a unique progressionStack for toolboxes
    //qDebug()<<"medReformatWorkspace::medReformatWorkspace";
    //this->createProgressionStack();

    this->addToolBox(d->reformatToolBox);

    connect(this->tabbedViewContainers(), SIGNAL(containersSelectedChanged()),
            this, SLOT(updateDataFromContainer()));
}

medReformatWorkspace::~medReformatWorkspace(void)
{
    delete d;
    d = NULL;
}

medReformatSelectorToolBox * medReformatWorkspace::reformatToolBox()
{
    return d->reformatToolBox;
}

void medReformatWorkspace::setupTabbedViewContainer()
{
    qDebug()<<"### medReformatWorkspace::setupTabbedViewContainer";

    if (!tabbedViewContainers()->count())
    {
        this->tabbedViewContainers()->addContainerInTabNamed(this->name());
    }
    this->tabbedViewContainers()->unlockTabs();
    this->tabbedViewContainers()->setKeepLeastOne(true);
}

bool medReformatWorkspace::isUsable()
{
    medToolBoxFactory * tbFactory = medToolBoxFactory::instance();
    return (tbFactory->toolBoxesFromCategory("reformat").size() != 0);
}

//void medReformatWorkspace::addViewEventFilter( medViewEventFilter* filter)
//{
//    qDebug()<<"### medReformatWorkspace::addViewEventFilter";

//    foreach(QUuid uuid, this->tabbedViewContainers()->containersSelected())
//    {
//        medViewContainer *container = medViewContainerManager::instance()->container(uuid);
//        if(container)
//        {
//            filter->installOnView(container->view());
//        }
//    }
//}

void medReformatWorkspace::updateDataFromContainer()
{
    qDebug()<<"### medReformatWorkspace::updateDataFromContainer";

    if(d->reformatToolBox)
    {
        qDebug()<<"### medReformatWorkspace::updateDataFromContainer ref ok";

        QList<medViewContainer*> containersList = tabbedViewContainers()->containersInTab(0);
        if (!containersList.isEmpty())
        {
            d->fixedContainer = containersList.at(tabbedViewContainers()->currentIndex());

            if(!d->fixedContainer->view())
            {
                d->reformatToolBox->setFixedData(NULL);
            }
            else
            {
                medAbstractLayeredView* fixedView  = dynamic_cast<medAbstractLayeredView*>(d->fixedContainer->view());
                if(!fixedView)
                {
                    dtkWarn() << "Non layered view are not suported yet in Reformat workspace.";
                    return;
                }

                medAbstractData *fixedData = fixedView->layerData(fixedView->currentLayer());

                qDebug()<<"### medReformatWorkspace::updateDataFromContainer set input data";

                if (!d->reformatToolBox->setFixedData(fixedData))
                {
                    // delete the view because something failed at some point
                    fixedView->deleteLater();
                }
            }
        }
    }
}

void medReformatWorkspace::updateUserLayerClosable(int tabIndex)
{
//    if(tabIndex == 0)
//        this->setUserLayerClosable(true);
//    else
//        this->setUserLayerClosable(false);
}

void medReformatWorkspace::updateFromReformatSuccess(medAbstractData *output)
{
   /* if(!d->reformatToolBox)
        return;

    //TODO disconnect because we dont want to change input of the undo redo process.
    //  find a better way to do it ? - RDE
    d->movingContainer->disconnect(this);

    d->movingContainer->removeView();
    d->movingContainer->addData(output);

    d->fuseContainer->removeView();
    d->fuseContainer->addData(d->ReformatToolBox->fixedData());
    d->fuseContainer->addData(output);


    // Relink the views...
    medAbstractLayeredView* movingView  = dynamic_cast<medAbstractLayeredView*>(d->movingContainer->view());
    if(!movingView)
    {
        dtkWarn() << "Non layered view are not suported yet in Reformat workspace.";
        return;
    }


    medAbstractLayeredView* fuseView  = dynamic_cast<medAbstractLayeredView*>(d->fuseContainer->view());
    if(!fuseView)
    {
        dtkWarn() << "Non layered view are not suported yet in Reformat workspace.";
        return;
    }

    d->viewGroup->addImpactedView(movingView);
    d->viewGroup->addImpactedView(fuseView);
    d->viewGroup->removeParameter("DataList");

    d->movingLayerGroup->addImpactedlayer(movingView, output);
    d->movingLayerGroup->addImpactedlayer(fuseView, output);

    connect(d->movingContainer,SIGNAL(viewContentChanged()),
            this, SLOT(updateFromMovingContainer()));

    connect(d->movingContainer,SIGNAL(viewRemoved()),
            this, SLOT(updateFromMovingContainer()));*/
}

void medReformatWorkspace::removeSelectorInternToolBox()
{
    d->reformatToolBox = NULL;
}
