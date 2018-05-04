/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractProcessLegacy.h>
#include <medAbstractData.h>
#include <dtkCoreSupport/dtkAbstractProcess.h>

#include "mscN4BiasCorrectionPluginExport.h"

class mscN4BiasCorrectionPrivate;

class MSCN4BIASCORRECTIONPLUGIN_EXPORT mscN4BiasCorrection : public medAbstractProcessLegacy
{
    Q_OBJECT
    
public:
    mscN4BiasCorrection();
    virtual ~mscN4BiasCorrection();
    
    virtual QString description() const;
    
    static bool registered();
    
public slots:
    
    //! Input data to the plugin is set through here
    void setInput(medAbstractData *data, int channel=0);
    
    //! Parameters are set through here, channel allows to handle multiple parameters
    void setParameter(double  data, int channel);
    
    //! Method to actually start the filter
    int update();
    
    //! The output will be available through here
    medAbstractData *output();
    
private:
    mscN4BiasCorrectionPrivate *d;
};

dtkAbstractProcess *createmscN4BiasCorrection();


