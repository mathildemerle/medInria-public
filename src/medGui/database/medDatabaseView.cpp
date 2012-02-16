/* medDatabaseView.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Tue Mar 31 13:18:20 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Wed Oct  6 19:08:51 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 126
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "medDatabaseView.h"
#include <medDataManager.h>
#include <medAbstractDatabaseItem.h>
#include <medAbstractDbController.h>

#include <QSortFilterProxyModel>
#include <QAbstractItemModel>

class NoFocusDelegate : public QStyledItemDelegate
{
public:
    NoFocusDelegate(medDatabaseView *view, QList<medDataIndex> indexes) : QStyledItemDelegate(), m_view(view), m_indexes(indexes) {}

    void append(medDataIndex);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    medDatabaseView *m_view;
    QList<medDataIndex> m_indexes;
};

void NoFocusDelegate::append(medDataIndex index)
{
    m_indexes.append(index);
}

void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus)
        itemOption.state = itemOption.state ^ QStyle::State_HasFocus;

    if(index.isValid()) {
        medAbstractDatabaseItem *item = NULL;

        if(QSortFilterProxyModel *proxy = dynamic_cast<QSortFilterProxyModel *>(m_view->model()))
            item = static_cast<medAbstractDatabaseItem *>(proxy->mapToSource(index).internalPointer());
        else if (QAbstractItemModel *model = dynamic_cast<QAbstractItemModel *>(m_view->model()))
            item = static_cast<medAbstractDatabaseItem *>(index.internalPointer());

        // items that failed to open will have a pinkish background
        if(item)
        {
            if (m_indexes.contains(item->dataIndex()))
               painter->fillRect(option.rect, QColor::fromRgb(qRgb(201, 121, 153)));

            medAbstractDbController * dbc = medDataManager::instance()->controllerForDataSource(item->dataIndex().dataSourceId());
            if ( dbc ) {
                if(!dbc->isPersistent())
                {
                    itemOption.font.setItalic(true);
                }
            }
        }
    }

    QStyledItemDelegate::paint(painter, itemOption, index);
}

class medDatabaseViewPrivate
{
public:
    QList<int> failedToOpenSeriesIds;
    QAction *viewAction;
    QAction *exportAction;
    QAction *saveAction;
    QAction *removeAction;
    QMenu *contextMenu;
};

medDatabaseView::medDatabaseView(QWidget *parent) : d(new medDatabaseViewPrivate), QTreeView(parent)
{
    this->setAcceptDrops(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setUniformRowHeights(true);
    this->setAlternatingRowColors(true);
    this->setAnimated(true);
    this->setSortingEnabled(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->header()->setStretchLastSection(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onItemDoubleClicked(const QModelIndex&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(updateContextMenu(const QPoint&)));

    NoFocusDelegate* delegate = new NoFocusDelegate(this, QList<medDataIndex>());
    this->setItemDelegate(delegate);

    d->contextMenu = new QMenu(this);

    //Init the QActions
    d->viewAction = new QAction(tr("View"), this);
    d->viewAction->setIconVisibleInMenu(true);
    d->viewAction->setIcon(QIcon(":icons/magnifier.svg"));
    connect(d->viewAction, SIGNAL(triggered()), this, SLOT(onMenuViewClicked()));

    d->exportAction = new QAction(tr("Export"), this);
    d->exportAction->setIconVisibleInMenu(true);
    d->exportAction->setIcon(QIcon(":icons/export.svg"));
    connect(d->exportAction, SIGNAL(triggered()), this, SLOT(onMenuExportClicked()));

    d->saveAction = new QAction(tr("Save"), this);
    d->saveAction->setIconVisibleInMenu(true);
    d->saveAction->setIcon(QIcon(":icons/save.svg"));
    connect(d->saveAction, SIGNAL(triggered()), this, SLOT(onMenuSaveClicked()));

    d->removeAction = new QAction(tr("Remove"), this);
    d->removeAction->setIconVisibleInMenu(true);
    d->removeAction->setIcon(QIcon(":icons/cross.svg"));
    connect(d->removeAction, SIGNAL(triggered()), this, SLOT(onMenuRemoveClicked()));
}

medDatabaseView::~medDatabaseView(void)
{
    //TreeViews don't take ownership of delegates
    delete this->itemDelegate();
    delete d;
}

int medDatabaseView::sizeHintForColumn(int column) const
{
    if (column<2)
        return 150;

    if (column == 2) // series description/filename
        return 250;

    if (column == 3) // slices count
        return 70;

    if (column == 5) // date of birth
        return 80;

    return 50;
}

void medDatabaseView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);

    for(int i = 0; i < model->columnCount(); this->resizeColumnToContents(i), i++);
    connect( this->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), SLOT(selectionChanged(const QModelIndex&, const QModelIndex&)));

}


void medDatabaseView::updateContextMenu(const QPoint& point)
{
    QModelIndex index = this->indexAt(point);

    if(!index.isValid())
        return;

    medAbstractDatabaseItem *item = NULL;

    if(QSortFilterProxyModel *proxy = dynamic_cast<QSortFilterProxyModel *>(this->model()))
        item = static_cast<medAbstractDatabaseItem *>(proxy->mapToSource(index).internalPointer());
    else if (QAbstractItemModel *model = dynamic_cast<QAbstractItemModel *>(this->model()))
        item = static_cast<medAbstractDatabaseItem *>(index.internalPointer());

    if (item) {
        d->contextMenu->clear();
        if( item->dataIndex().isValidForSeries())
        {
            d->contextMenu->addAction(d->viewAction);
            d->contextMenu->addAction(d->exportAction);
            d->contextMenu->addAction(d->removeAction);
            if( !(medDataManager::instance()->controllerForDataSource(item->dataIndex().dataSourceId())->isPersistent()) )
                d->contextMenu->addAction(d->saveAction);
            d->contextMenu->exec(mapToGlobal(point));
        }
        else if (item->dataIndex().isValidForPatient())
        {
            d->contextMenu->addAction(d->removeAction);
            if( !(medDataManager::instance()->controllerForDataSource(item->dataIndex().dataSourceId())->isPersistent()) )
                d->contextMenu->addAction(d->saveAction);
            d->contextMenu->exec(mapToGlobal(point));
        }
    }
}

void medDatabaseView::onItemClicked(const QModelIndex& index)
{

    medAbstractDatabaseItem *item = NULL;

    if(QSortFilterProxyModel *proxy = dynamic_cast<QSortFilterProxyModel *>(this->model()))
        item = static_cast<medAbstractDatabaseItem *>(proxy->mapToSource(index).internalPointer());
    else if (QAbstractItemModel *model = dynamic_cast<QAbstractItemModel *>(this->model()))
        item = static_cast<medAbstractDatabaseItem *>(index.internalPointer());

    if (!item)
        return;

    if (item->dataIndex().isValidForSeries())
    {
        this->collapseAll();
        this->setExpanded(index.parent().parent(), true);
        this->setExpanded(index.parent(), true);
        this->setExpanded(index, true);
        emit seriesClicked(item->dataIndex ());
    }
    else if (item->dataIndex().isValidForPatient())
    {
        this->collapseAll();
        this->setExpanded(index, true);
        emit patientClicked(item->dataIndex ());
    }


}

void medDatabaseView::onItemDoubleClicked(const QModelIndex& index)
{
    medAbstractDatabaseItem *item = NULL;

    if(QSortFilterProxyModel *proxy = dynamic_cast<QSortFilterProxyModel *>(this->model()))
        item = static_cast<medAbstractDatabaseItem *>(proxy->mapToSource(index).internalPointer());
    else if (QAbstractItemModel *model = dynamic_cast<QAbstractItemModel *>(this->model()))
        item = static_cast<medAbstractDatabaseItem *>(index.internalPointer());

    if (item)
        emit (open(item->dataIndex()));
}

void medDatabaseView::onMenuViewClicked(void)
{
    if(!this->selectedIndexes().count())
        return;

    QModelIndex index = this->selectedIndexes().first();

    if(!index.isValid())
        return;

    medAbstractDatabaseItem *item = NULL;

    if(QSortFilterProxyModel *proxy = dynamic_cast<QSortFilterProxyModel *>(this->model()))
        item = static_cast<medAbstractDatabaseItem *>(proxy->mapToSource(index).internalPointer());

    if (item && (item->dataIndex().isValidForSeries()))
    {
        emit open(item->dataIndex ());
    }
}

void medDatabaseView::onMenuExportClicked(void)
{
    if(!this->selectedIndexes().count())
        return;

    QModelIndex index = this->selectedIndexes().first();

    if(!index.isValid())
        return;

    medAbstractDatabaseItem *item = NULL;

    if(QSortFilterProxyModel *proxy = dynamic_cast<QSortFilterProxyModel *>(this->model()))
        item = static_cast<medAbstractDatabaseItem *>(proxy->mapToSource(index).internalPointer());

    if(item)
        emit exportData(item->dataIndex());
}

void medDatabaseView::selectionChanged( const QModelIndex& current, const QModelIndex& previous)
{
    emit onItemClicked(current);
}

void medDatabaseView::onMenuRemoveClicked( void )
{
    int reply = QMessageBox::question(this, tr("Remove item"),
            tr("Are you sure you want to continue?\n""This cannot be undone."),
            QMessageBox::Yes, QMessageBox::No, QMessageBox::NoButton);

    if( reply == QMessageBox::Yes )
    {
        QModelIndexList indexes = this->selectedIndexes();
        if(!indexes.count())
            return;

        QModelIndex index = indexes.at(0);

        medAbstractDatabaseItem *item = NULL;

        if(QSortFilterProxyModel *proxy = dynamic_cast<QSortFilterProxyModel *>(this->model()))
            item = static_cast<medAbstractDatabaseItem *>(proxy->mapToSource(index).internalPointer());

        if (item)
        {
            // Copy the data index, because the data item may cease to be valid.
            medDataIndex index = item->dataIndex();
            medDataManager::instance()->removeData(index);
        }
    }
}

void medDatabaseView::onMenuSaveClicked(void)
{
        QModelIndexList indexes = this->selectedIndexes();
        if(!indexes.count())
            return;

        QModelIndex index = indexes.at(0);

        medAbstractDatabaseItem *item = NULL;

        if(QSortFilterProxyModel *proxy = dynamic_cast<QSortFilterProxyModel *>(this->model()))
            item = static_cast<medAbstractDatabaseItem *>(proxy->mapToSource(index).internalPointer());

        if (item)
        {
            // Copy the data index, because the data item may cease to be valid.
            medDataIndex index = item->dataIndex();
            medDataManager::instance()->storeNonPersistentMultipleDataToDatabase(index);
            qDebug() << "DEBUG : onMenuSaveClicked() after storeNonPersistentSingleDataToDatabase";
            qDebug() << "DEBUG : index" << index;
        }
}

void medDatabaseView::onOpeningFailed(const medDataIndex& index)
{
    if (NoFocusDelegate* delegate =
            dynamic_cast<NoFocusDelegate*>(this->itemDelegate()))
    {

        delegate->append(index);
    }
}
