#include "medToolBoxDiffusionFiberBundling.h"

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractDataFactory.h>
#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewInteractor.h>

#include <medCore/medDataManager.h>
//#include <medSql/medDatabaseController.h>

class medToolBoxDiffusionFiberBundlingPrivate
{
public:
    //QListWidget  *bundlingList;
    QListView  *bundlingList;
    QStandardItemModel *bundlingModel;
    QPushButton  *bundlingButtonVdt;
    QPushButton  *bundlingButtonTag;
    QPushButton  *bundlingButtonRst;
    QCheckBox    *bundlingShowCheckBox;
    QCheckBox    *bundleBoxCheckBox;
    QPushButton  *bundlingButtonAdd;
    
    dtkAbstractView *view;
    dtkAbstractData *data;
};

medToolBoxDiffusionFiberBundling::medToolBoxDiffusionFiberBundling(QWidget *parent) : medToolBox(parent), d(new medToolBoxDiffusionFiberBundlingPrivate)
{
    d->view = 0;
    d->data = 0;
    
    QWidget *bundlingPage = new QWidget(this);
    
    d->bundlingButtonTag = new QPushButton("Tag", bundlingPage);
    d->bundlingButtonAdd = new QPushButton("Add", bundlingPage);
    d->bundlingButtonRst = new QPushButton("Reset", bundlingPage);
    d->bundlingButtonVdt = new QPushButton("Validate", bundlingPage);
    
    QHBoxLayout *bundlingButtonsLayout = new QHBoxLayout;
    bundlingButtonsLayout->addWidget(d->bundlingButtonTag);
    bundlingButtonsLayout->addWidget(d->bundlingButtonAdd);
    bundlingButtonsLayout->addWidget(d->bundlingButtonRst);
    bundlingButtonsLayout->addWidget(d->bundlingButtonVdt);
    
    // d->bundlingList = new QListWidget(bundlingPage);
    
    d->bundlingModel = new QStandardItemModel(0, 1, bundlingPage);
    d->bundlingModel->setHeaderData(0, Qt::Horizontal, tr("Fiber bundles"));
            
    QItemSelectionModel *selectionModel = new QItemSelectionModel(d->bundlingModel);
    
    d->bundlingList = new QListView(bundlingPage);
    d->bundlingList->setAlternatingRowColors(true);
    d->bundlingList->setModel (d->bundlingModel);
    d->bundlingList->setSelectionModel(selectionModel);
    
    d->bundlingShowCheckBox = new QCheckBox("Show all bundles", bundlingPage);
    d->bundleBoxCheckBox = new QCheckBox("Activate bundling box", bundlingPage);
        
    QVBoxLayout *bundlingLayout = new QVBoxLayout(bundlingPage);
    bundlingLayout->addWidget(d->bundleBoxCheckBox);
    bundlingLayout->addLayout(bundlingButtonsLayout);
    bundlingLayout->addWidget(d->bundlingList);
    bundlingLayout->addWidget(d->bundlingShowCheckBox);

    connect (d->bundlingButtonVdt,     SIGNAL(clicked(void)),            this, SLOT (onBundlingButtonVdtClicked (void)));
    connect (d->bundleBoxCheckBox,     SIGNAL(toggled(bool)),            this, SLOT (onBundleBoxCheckBoxToggled (bool)));
	
    connect (d->bundlingShowCheckBox,  SIGNAL(toggled(bool)),            this, SIGNAL (showBundles (bool)));
    connect (d->bundlingButtonTag,     SIGNAL(clicked(void)),            this, SIGNAL (fiberSelectionTagged(void)));
    connect (d->bundlingButtonRst,     SIGNAL(clicked(void)),            this, SIGNAL (fiberSelectionReset(void)));
 
    this->setTitle("Fiber Bundling");
    this->addWidget(bundlingPage);
}

medToolBoxDiffusionFiberBundling::~medToolBoxDiffusionFiberBundling()
{
    delete d;
    d = NULL;
}

void medToolBoxDiffusionFiberBundling::setData(dtkAbstractData *data)
{
    if (!data)
        return;
    
    if (data->description()!="v3dDataFibers") {
        return;
    }
    
    if (d->data==data)
        return;
    
    d->data = data;
    
    //d->bundlingList->clear();
    //d->bundlingModel->clear();
    d->bundlingModel->removeRows(0, d->bundlingModel->rowCount(QModelIndex()), QModelIndex());
    
    qDebug() << "Here!!";
    
    if (data->hasMetaData("BundleList") && data->hasMetaData("BundleColorList")) {
        
        QStringList bundleNames  = data->metaDataValues("BundleList");
        QStringList bundleColors = data->metaDataValues("BundleColorList");
        if (bundleNames.count()==bundleColors.count()) {
            for (int i=0; i<bundleNames.count(); i++)
                this->addBundle(bundleNames[i], QColor(bundleColors[i]));
        }
    }
}

void medToolBoxDiffusionFiberBundling::onBundlingButtonVdtClicked (void)
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter bundle name"),
                                         tr(""), QLineEdit::Normal, tr(""), &ok);
    
    if (ok && !text.isEmpty()) {
        // if (!d->bundlingList->contains (name)) // should popup a warning
        QColor color = QColor::fromHsv(qrand()%360, 255, 210);
        
        emit fiberSelectionValidated (text, color);
        this->addBundle (text, color);
    }
}

void medToolBoxDiffusionFiberBundling::onBundleBoxCheckBoxToggled (bool value)
{
    if (!d->view)
        return;

    if (dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
        if (value)
	    interactor->setProperty ("BoxVisibility", "true");
        else
            interactor->setProperty ("BoxVisibility", "false");
                
	d->view->update();
    } 
}

void medToolBoxDiffusionFiberBundling::addBundle (const QString &name, const QColor &color)
{/*
    if(!d->data)
        return;
  */
    //d->bundlingList->addItem (name);
    
    //d->bundlingModel->removeRows(0, d->bundlingModel->rowCount(QModelIndex()), QModelIndex());
    
    int row = d->bundlingModel->rowCount();
    QStandardItem *item = new QStandardItem;
    item->setCheckable(true);
    d->bundlingModel->insertRow(row, item);
    
    //d->bundlingModel->addItem (name);
    
    d->bundlingModel->setData(d->bundlingModel->index(row, 0, QModelIndex()),
                              name);
    
    d->bundlingModel->setData(d->bundlingModel->index(row, 0, QModelIndex()),
                              true, Qt::CheckStateRole);

    d->bundlingModel->setData(d->bundlingModel->index(row, 0, QModelIndex()),
                              color, Qt::DecorationRole);

    //d->bundlingList->update();
}

void medToolBoxDiffusionFiberBundling::clear(void)
{
    //d->bundlingList->clear();
    //d->bundlingModel->clear();
    d->bundlingModel->removeRows(0, d->bundlingModel->rowCount(QModelIndex()), QModelIndex());

    //this->update (0);
    d->view = 0;
    d->data = 0;
}

void medToolBoxDiffusionFiberBundling::update(dtkAbstractView *view)
{
    if (d->view==view) {
        if (view)
            if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor"))
                this->setData (interactor->data()); // data may have changed
        return;
    }
    
    d->bundlingModel->removeRows(0, d->bundlingModel->rowCount(QModelIndex()), QModelIndex());
    
    if (d->view) {
        if (dtkAbstractViewInteractor *interactor = d->view->interactor ("v3dViewFiberInteractor")) {
            disconnect (this, SIGNAL(fiberSelectionValidated(const QString&, const QColor&)), 
                        interactor, SLOT(onSelectionValidated(const QString&, const QColor&)));
            disconnect (this, SIGNAL(fiberSelectionTagged()),    interactor, SLOT(onSelectionTagged()));
            disconnect (this, SIGNAL(fiberSelectionReset()),     interactor, SLOT(onSelectionReset()));
        }
    }
    
    if (!view) {
        d->view = 0;
        d->data = 0;
        return;
    }
    
    if (view->property ("Orientation")!="3D") { // only interaction with 3D views is authorized
        d->view = 0;
        d->data = 0;
        return;
    }
    
    d->view = view;
    
    if (dtkAbstractViewInteractor *interactor = view->interactor ("v3dViewFiberInteractor")) {
        connect (this, SIGNAL(fiberSelectionValidated(const QString&, const QColor&)), 
                 interactor, SLOT(onSelectionValidated(const QString&, const QColor&)));
        connect (this, SIGNAL(fiberSelectionTagged()),           interactor, SLOT(onSelectionTagged()));
        connect (this, SIGNAL(fiberSelectionReset()),            interactor, SLOT(onSelectionReset()));

        d->bundleBoxCheckBox->blockSignals (true);
        d->bundleBoxCheckBox->setChecked( interactor->property("BoxVisibility")=="true" );
        d->bundleBoxCheckBox->blockSignals (false);
        
        this->setData (interactor->data());
    }
}
