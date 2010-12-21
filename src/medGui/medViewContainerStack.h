/* medViewContainerStack.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Thu May 13 12:37:58 2010 (+0200)
 * Version: $Id$
 * Last-Updated: Thu May 13 12:39:00 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 3
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MEDVIEWCONTAINERSTACK_H
#define MEDVIEWCONTAINERSTACK_H

#include <QtGui>

#include "medGuiExport.h"

class dtkAbstractView;
class medDataIndex;
class medViewContainer;
class medViewContainerStackPrivate;

class MEDGUI_EXPORT medViewContainerStack : public QStackedWidget
{
    Q_OBJECT

public:
     medViewContainerStack(QWidget *parent = 0);
    ~medViewContainerStack(void);

    medViewContainer *current(void);

    medViewContainer *single(void);
    medViewContainer *multi(void);
    medViewContainer *custom(void);
    medViewContainer *compare (void);
    medViewContainer *fuse (void);
    
    void addCustomContainer(const QString &name, medViewContainer *container);
    medViewContainer* customContainer(const QString &name);
    
    void setCustomContainer(const QString &name);

signals:
    void dropped(const medDataIndex&);
    void focused(dtkAbstractView *);

private:
    medViewContainerStackPrivate *d;
};

#endif
