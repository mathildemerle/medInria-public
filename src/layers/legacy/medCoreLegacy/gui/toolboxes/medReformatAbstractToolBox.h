/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medToolBox.h>

#include <medCoreLegacyExport.h>

class medReformatSelectorToolBox;
class medReformatAbstractToolBoxPrivate;

class MEDCORELEGACY_EXPORT medReformatAbstractToolBox : public medToolBox
{
    Q_OBJECT

public:
    medReformatAbstractToolBox(QWidget *parentToolBox = 0);
    virtual ~medReformatAbstractToolBox();

protected:
    medReformatSelectorToolBox *parentToolBox();

private:
    medReformatAbstractToolBoxPrivate *d;
};


