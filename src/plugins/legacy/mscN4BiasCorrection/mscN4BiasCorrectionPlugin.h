/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medPlugin.h>
#include "mscN4BiasCorrectionPluginExport.h"

class MSCN4BIASCORRECTIONPLUGIN_EXPORT mscN4BiasCorrectionPlugin : public medPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.inria.mscN4BiasCorrectionPlugin" FILE "mscN4BiasCorrectionPlugin.json")
    Q_INTERFACES(dtkPlugin)

public:
    mscN4BiasCorrectionPlugin(QObject *parent = 0);
    virtual bool initialize();
    
    virtual QString description() const;
    virtual QString name() const;
    virtual QString version() const;
    virtual QStringList types() const;
};


