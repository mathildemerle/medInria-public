/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medReformatAbstractToolBox.h>

class mscN4BiasCorrectionToolBoxPrivate;

class mscN4BiasCorrectionToolBox : public medReformatAbstractToolBox
{
    Q_OBJECT
    MED_TOOLBOX_INTERFACE("N4 Bias Correction",
                          "Apply the N4 bias field correction algorithm",
                          <<"reformat")

public:
    mscN4BiasCorrectionToolBox(QWidget *parent = 0);
    ~mscN4BiasCorrectionToolBox();
    
    medAbstractData *processOutput();
    
    static bool registered();
    dtkPlugin * plugin();
    std::vector<int> extractValue(QString text);

public slots:
    void update(medAbstractData*){}
    void run();
    
signals:
    void success();
    void failure();
    
private:
    mscN4BiasCorrectionToolBoxPrivate *d;
};


