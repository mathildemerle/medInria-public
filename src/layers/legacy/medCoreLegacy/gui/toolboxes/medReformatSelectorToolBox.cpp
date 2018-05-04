/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medReformatSelectorToolBox.h>

#include <dtkCoreSupport/dtkAbstractViewFactory.h>
#include <medAbstractDataFactory.h>
#include <medAbstractData.h>
#include <dtkCoreSupport/dtkAbstractProcessFactory.h>
#include <dtkCoreSupport/dtkAbstractProcess.h>
#include <dtkCoreSupport/dtkAbstractViewInteractor.h>
#include <dtkCoreSupport/dtkSmartPointer.h>

#include <medDataManager.h>
#include <medJobManagerL.h>
#include <medMessageController.h>
#include <medMetaDataKeys.h>
#include <medAbstractView.h>
#include <medAbstractImageView.h>
//#include <medAbstractReformatProcess.h>

#include <medAbstractImageData.h>
#include <medRunnableProcess.h>
#include <medToolBoxTab.h>
#include <medToolBoxFactory.h>
#include <medToolBoxHeader.h>
#include <medButton.h>
#include <medViewEventFilter.h>

#include <medReformatAbstractToolBox.h>

#include <QtGui>

class medReformatSelectorToolBoxPrivate
{
public:
    QComboBox *chooseReformatComboBox;
    medAbstractData *inputData;
    medReformatAbstractToolBox * currentReformatToolBox;
    QHash<QString, medReformatAbstractToolBox*> reformatToolBoxes;
    QVBoxLayout *mainLayout;
};

medReformatSelectorToolBox::medReformatSelectorToolBox(QWidget *parent) : medToolBox(parent), d(new medReformatSelectorToolBoxPrivate)
{
    d->currentReformatToolBox = nullptr;

    d->chooseReformatComboBox = new QComboBox;
    //TODO algorithm is not the best IMO - RDE
    d->chooseReformatComboBox->addItem("Choose algorithm");
    d->chooseReformatComboBox->setToolTip(tr("Browse through the list of available Reformat algorithm"));

    medToolBoxFactory* tbFactory = medToolBoxFactory::instance();
    int i = 1; //account for the choose Filter item
    foreach(QString toolboxName, tbFactory->toolBoxesFromCategory("reformat"))
    {
        qDebug()<<"### medReformatSelectorToolBox name "<<toolboxName;

        medToolBoxDetails* details = tbFactory->toolBoxDetailsFromId(toolboxName);
        d->chooseReformatComboBox->addItem(details->name, toolboxName);
        d->chooseReformatComboBox->setItemData(i,
                                                   details->description,
                                                   Qt::ToolTipRole);
        i++;
    }

    connect(d->chooseReformatComboBox, SIGNAL(activated(int)), this, SLOT(changeCurrentToolBox(int)));

    QWidget *mainWidget = new QWidget;
    d->mainLayout = new QVBoxLayout;

    d->chooseReformatComboBox->adjustSize();
    d->mainLayout->addWidget(d->chooseReformatComboBox);
    mainWidget->setLayout(d->mainLayout);
    this->addWidget(mainWidget);
    this->setTitle("Reformat");

    d->inputData = nullptr;
    qDebug()<<"### medReformatSelectorToolBox end.";
}

medReformatSelectorToolBox::~medReformatSelectorToolBox(void)
{
    delete d;
}

//! Gets the fixedData.
medAbstractData *medReformatSelectorToolBox::fixedData(void)
{
    return d->inputData;
}

bool medReformatSelectorToolBox::setFixedData(medAbstractData* data)
{
    d->inputData = data;

    return true;
}

/**
 * Sets up the toolbox chosen and remove the old one.
 *
 * @param index The index of the toolbox that was chosen.
 */
void medReformatSelectorToolBox::changeCurrentToolBox(int index)
{
    medReformatAbstractToolBox *toolbox = nullptr;

    //get identifier for toolbox.
    QString identifier = d->chooseReformatComboBox->itemData(index).toString();
    if (d->reformatToolBoxes.contains (identifier))
        toolbox = d->reformatToolBoxes[identifier];
    else
    {
        medToolBox* tb = medToolBoxFactory::instance()->createToolBox(identifier, this);
        toolbox = qobject_cast<medReformatAbstractToolBox*>(tb);
        if (toolbox)
        {
            toolbox->setStyleSheet("medToolBoxBody {border:none}");
            d->reformatToolBoxes[identifier] = toolbox;
//            connect(toolbox, SIGNAL(installEventFilterRequest(medViewEventFilter*)),
//                    this, SIGNAL(installEventFilterRequest(medViewEventFilter*)),
//                    Qt::UniqueConnection);
        }
    }

    if(d->currentReformatToolBox)
    {
        d->currentReformatToolBox->hide();
        d->mainLayout->removeWidget(d->currentReformatToolBox);
        d->currentReformatToolBox = NULL;
    }

    if(!toolbox)
    {
        this->setAboutPluginVisibility(false);
        return;
    }


    d->currentReformatToolBox = toolbox;
    d->currentReformatToolBox->header()->hide();

    //dtkPlugin* plugin = d->currentReformatToolBox->plugin();
    //this->setAboutPluginButton(plugin);
    this->setAboutPluginVisibility(true);

    d->currentReformatToolBox->show();
    d->mainLayout->addWidget(d->currentReformatToolBox);

    connect ( d->currentReformatToolBox, SIGNAL(success()), this, SIGNAL(success()));
}

//! Clears the toolbox.
void medReformatSelectorToolBox::clear(void)
{
    //maybe clear the currentToolBox?
//    if (d->currentToolBox)
//        d->currentToolBox->clear();
}

//! Gets the process.
//medAbstractReformatProcess * medReformatSelectorToolBox::process(void)
//{
//    return d->process;
//}

/**
 * Sets the process.
 *
 * @param proc The new process.
 */
//void medReformatSelectorToolBox::setProcess(medAbstractReformatProcess* proc)
//{
//    d->process = proc;
//}

//QString medReformatSelectorToolBox::getNameOfCurrentAlgorithm()
//{
//    return d->nameOfCurrentAlgorithm;
//}

/*void medReformatSelectorToolBox::handleOutput(typeOfOperation type, QString algoName)
{
    medAbstractData *output(NULL);
    if (type == algorithm)
        if (d->process)
            output = dynamic_cast<medAbstractData*>(d->process->output());
        else return;
    else
        if (d->undoRedoProcess)
            output = dynamic_cast<medAbstractData*>(d->undoRedoProcess->output());
        else return;

    // We manage the new description of the image
    QString newDescription = "";
    if(d->movingData)
        newDescription = d->movingData->metadata(medMetaDataKeys::SeriesDescription.key());

    if (type==algorithm || type==redo)
    {
        if (type==algorithm)
        {
            algoName = d->nameOfCurrentAlgorithm.remove(" "); // we remove the spaces to reduce the size of the QString as much as possible
        }
        if (newDescription.contains("registered"))
            newDescription += "-" + algoName + "\n";
        else
            newDescription += " registered\n-" + algoName+ "\n";
    }
    else if (type == undo)
    {
        newDescription.remove(newDescription.lastIndexOf("-"),newDescription.size()-1);
        if (newDescription.count("-") == 0)
            newDescription.remove(" registered\n");
    }
    else if (type == reset)
    {
        if (newDescription.lastIndexOf(" registered") != -1)
            newDescription.remove(newDescription.lastIndexOf(" registered"),newDescription.size()-1);
        if(!d->fixedData || !d->movingData)
            return;
    }

    foreach(QString metaData, d->fixedData->metaDataList())
        output->addMetaData(metaData,d->fixedData->metaDataValues(metaData));

    foreach(QString property, d->fixedData->propertyList())
        output->addProperty(property,d->fixedData->propertyValues(property));

    output->setMetaData(medMetaDataKeys::SeriesDescription.key(), newDescription);

    QString generatedID = QUuid::createUuid().toString().replace("{","").replace("}","");
    output->setMetaData ( medMetaDataKeys::SeriesID.key(), generatedID );

    if (type==algorithm)
        medDataManager::instance()->importData(output);

    d->movingData = output;
    emit movingDataRegistered(output);
}*/

void medReformatSelectorToolBox::enableSelectorToolBox(bool enable)
{
    this->setEnabled(enable);
}

//void medReformatSelectorToolBox::onJobAdded(medJobItemL* item, QString jobName)
//{
//    if (d->process)
//        if (jobName == d->process->identifier()){
//            dtkAbstractProcess * proc = static_cast<medRunnableProcess*>(item)->getProcess();
//            if (proc==d->process)
//                enableSelectorToolBox(false);
//        }
//}


