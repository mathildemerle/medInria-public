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
#include <medJobItemL.h>
#include <medViewEventFilter.h>

#include <medCoreLegacyExport.h>

class medAbstractImageView;
class medAbstractImageData;
class medDataIndex;
class medReformatSelectorToolBoxPrivate;

class MEDCORELEGACY_EXPORT medReformatSelectorToolBox : public medToolBox
{
    Q_OBJECT

public:
     medReformatSelectorToolBox(QWidget *parent = 0);
    ~medReformatSelectorToolBox();

    medAbstractData *fixedData();

    bool setFixedData(medAbstractData* data);

signals:
    void showError (const QString&,unsigned int timeout);
    void showInfo(const QString&,unsigned int timeout);
    void movingDataRegistered(medAbstractData *output);
    //void installEventFilterRequest(medViewEventFilter*);

public slots:

    void changeCurrentToolBox(int index);
    void clear();
    //void onSaveTrans();
    //void handleOutput(typeOfOperation type=algorithm,QString algoName="");
    void enableSelectorToolBox(bool enable = true);
    //void onJobAdded(medJobItemL* item, QString jobName);

private:
    medReformatSelectorToolBoxPrivate *d;
};


