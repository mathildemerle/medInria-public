/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "mscN4BiasCorrection.h"
#include "mscN4BiasCorrectionPlugin.h"
#include "mscN4BiasCorrectionToolBox.h"

mscN4BiasCorrectionPlugin::mscN4BiasCorrectionPlugin(QObject *parent) : medPlugin(parent)
{
}

bool mscN4BiasCorrectionPlugin::initialize()
{
    if(!mscN4BiasCorrection::registered())
    {
        dtkWarn() << "Unable to register mscN4BiasCorrection type";
    }

    if ( !mscN4BiasCorrectionToolBox::registered() )
    {
        dtkWarn() << "Unable to register mscN4BiasCorrection toolbox";
    }
    return true;
}

QString mscN4BiasCorrectionPlugin::description() const
{
  QString description = \
          "This plugin implements <a href=\"https://itk.org/Doxygen/html/classitk_1_1N4BiasFieldCorrectionImageFilter.html\" style=\"color: #cc0000\" >itk::N4BiasFieldCorrectionImageFilter</a> \
          from the <a href=\"https://itk.org/\" style=\"color: #cc0000\" >ITK library</a>.<br>  \
          Drop an MRI volume in the INPUT view, choose your parameters and run.";
  return description;
}

QString mscN4BiasCorrectionPlugin::name() const
{
    return "N4 Bias Correction";
}

QString mscN4BiasCorrectionPlugin::version() const
{
    return MSCN4BIASCORRECTIONPLUGIN_VERSION;
}

QStringList mscN4BiasCorrectionPlugin::types() const
{
    return QStringList() << "mscN4BiasCorrection";
}
