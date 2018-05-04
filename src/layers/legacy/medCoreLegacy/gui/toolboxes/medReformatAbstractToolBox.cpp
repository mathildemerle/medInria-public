/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medReformatSelectorToolBox.h>
#include <medReformatAbstractToolBox.h>

class medReformatAbstractToolBoxPrivate
{
public:
    medReformatSelectorToolBox *parent;
};

medReformatAbstractToolBox::medReformatAbstractToolBox(QWidget *parent) : medToolBox(parent), d(new medReformatAbstractToolBoxPrivate)
{
    d->parent = qobject_cast<medReformatSelectorToolBox*>(parent);
}

medReformatAbstractToolBox::~medReformatAbstractToolBox(void)
{
    delete d;
}

//void medReformatAbstractToolBox::setReformatToolBox(medReformatSelectorToolBox *toolbox)
//{
//    d->parent = toolbox;
//}

medReformatSelectorToolBox *medReformatAbstractToolBox::parentToolBox(void)
{
    return d->parent;
}
