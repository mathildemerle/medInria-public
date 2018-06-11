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

class medViewEventFilter;
class medSegmentationAbstractToolBox;

class medSegmentationSelectorToolBoxPrivate;
class MEDCORELEGACY_EXPORT medSegmentationSelectorToolBox : public medToolBox
{
    Q_OBJECT

public:
     medSegmentationSelectorToolBox(QWidget *parent = 0);
    ~medSegmentationSelectorToolBox();

     medAbstractData* data();

     medSegmentationAbstractToolBox* currentToolBox();

signals:
     void installEventFilterRequest(medViewEventFilter *filter);
     void inputChanged();

public slots:
    void changeCurrentToolBox(int index);


private:
    medSegmentationSelectorToolBoxPrivate *d;
};


