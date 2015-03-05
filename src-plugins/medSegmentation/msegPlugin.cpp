/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <msegPlugin.h>

#include <msegAlgorithmInitializer.h>
#include <msegAnnotationInteractor.h>
#include <msegAlgorithmPaintToolbox.h>

#include <dtkLog/dtkLog.h>

// /////////////////////////////////////////////////////////////////
// PluginPrivate
// /////////////////////////////////////////////////////////////////

class PluginPrivate
{
public:
    // Class variables go here.
    static const char *s_Name;
};
const char * PluginPrivate::s_Name = "segmentationPlugin";

// /////////////////////////////////////////////////////////////////
// Plugin
// /////////////////////////////////////////////////////////////////

Plugin::Plugin(QObject *parent) : dtkPlugin(parent), d(new PluginPrivate)
{

}

Plugin::~Plugin()
{
    delete d;
    d = NULL;
}

bool Plugin::initialize()
{
    if(!msegAnnotationInteractor::registered())
    {
        qDebug() << "Unable to register v3dViewAnnotationInteractor";
    }
    if(!AlgorithmPaintToolbox::registered())
    {
        qDebug() << "Unable to register msegAlgorithmPaintToolbox";
    }

    if(!AlgorithmPaintToolbox::registered())
    {
        qDebug() << "Unable to register msegAlgorithmPaintToolbox";
    }

    return AlgorithmInitializer::initialize();
}

bool Plugin::uninitialize()
{
    return true;
}

QString Plugin::name() const
{
    return PluginPrivate::s_Name;
}

QString Plugin::description() const
{
    return tr("Segmentation plugin<br/>"
              "Manual painting of regions: inside, and outside."
              "<br/> Exports masks with three values: inside (1), outside (2)"
              "and other (0). You may leave out the outside colour "
              "for simple binary masks.<br/>"
              "Ctrl+z         Undo <br/>"
              "Ctrl+y         Redo <br/>"
              "Ctrl+c         Copy <br/>"
              "Ctrl+v         Paste<br/>"
              "Ctrl+Backspace Remove seed<br/>"
              "Ctrl+Up        Add brush size<br/>"
              "Ctrl+Down      Reduce brush size");
    
}

QString Plugin::version() const
{
    return MEDSEGMENTATIONPLUGIN_VERSION;
}

QString Plugin::contact() const
{
    return "John.Stark@inria.fr";
}

QStringList Plugin::authors() const
{
    QStringList list;
    list << "John Stark";
    return list;
}

QStringList Plugin::contributors() const
{
    QStringList list;
    list <<  QString::fromUtf8("Benoît Bleuzé")
             << "Olivier Commowick";
    return list;
}

QString Plugin::identifier() const
{
    return PluginPrivate::s_Name;
}

QStringList Plugin::tags() const
{
    return QStringList();
}

QStringList Plugin::types() const
{
    return QStringList();
}

Q_EXPORT_PLUGIN2(Plugin, Plugin)