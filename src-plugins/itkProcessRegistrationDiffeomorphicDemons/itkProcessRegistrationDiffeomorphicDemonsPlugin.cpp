/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <itkProcessRegistrationDiffeomorphicDemons.h>
#include <itkProcessRegistrationDiffeomorphicDemonsPlugin.h>
#include <itkProcessRegistrationDiffeomorphicDemonsToolBox.h>

itkProcessRegistrationDiffeomorphicDemonsPlugin::itkProcessRegistrationDiffeomorphicDemonsPlugin(QObject *parent) : medPlugin(parent)
{
}

bool itkProcessRegistrationDiffeomorphicDemonsPlugin::initialize()
{
    if (!itkProcessRegistrationDiffeomorphicDemons::registered())        { dtkWarn() << "Unable to register itkProcessRegistrationDiffeomorphicDemons type";    }
    if (!itkProcessRegistrationDiffeomorphicDemonsToolBox::registered()) { dtkWarn() << "Unable to register itkProcessRegistrationDiffeomorphicDemons toolbox"; }

    return true;
}

QString itkProcessRegistrationDiffeomorphicDemonsPlugin::name() const
{
    return "Diffeomorphic Demons";
}

QStringList itkProcessRegistrationDiffeomorphicDemonsPlugin::authors() const
{
    QStringList list;
    list << QString::fromUtf8("Benoît Bleuzé");
    return list;
}

QStringList itkProcessRegistrationDiffeomorphicDemonsPlugin::contributors() const
{
    QStringList list;
    list << "Vincent Garcia";
    list << medPlugin::contributors();
    return list;
}

QString itkProcessRegistrationDiffeomorphicDemonsPlugin::description() const
{
    QString description = \
            tr("Applies the diffeomorphic demons as they can be found in the <a href=\"https://github.com/Inria-Asclepios/RPI\" style=\"color: #cc0000\" >RPI library</a>. <br><br>\
               Converts any type of image to float before applying the change, since the diffeomorphic demons algorithm only works on float images. \
               <br><br>See: <a href=\"http://www.insight-journal.org/browse/publication/154\" style=\"color: #cc0000\" > http://www.insight-journal.org/browse/publication/154 </a> \
            <br><br>This plugin uses the <a href=\"https://itk.org/\" style=\"color: #cc0000\" >ITK library</a>.");
    return description;
}

QString itkProcessRegistrationDiffeomorphicDemonsPlugin::version() const
{
    return ITKPROCESSREGISTRATIONDIFFEOMORPHICDEMONSPLUGIN_VERSION;
}

QStringList itkProcessRegistrationDiffeomorphicDemonsPlugin::types() const
{
    return QStringList() << "itkProcessRegistrationDiffeomorphicDemons";
}

Q_EXPORT_PLUGIN2(itkProcessRegistrationDiffeomorphicDemonsPlugin, itkProcessRegistrationDiffeomorphicDemonsPlugin)


