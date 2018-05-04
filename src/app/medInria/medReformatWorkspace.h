/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once


#include <QtCore>

#include <medAbstractWorkspaceLegacy.h>
#include <medReformatSelectorToolBox.h>
#include <medViewEventFilter.h>

class medTabbedViewContainers;
class medReformatWorkspacePrivate;
class medAbstractData;

class medReformatWorkspace : public medAbstractWorkspaceLegacy
{
    Q_OBJECT
    MED_WORKSPACE_INTERFACE("Reformat",
                            "Workspace to compute the spatial transformation between two images in order to register them.",
                            "Methodology")
public:
     medReformatWorkspace(QWidget *parent);
    ~medReformatWorkspace();

    static bool isUsable();
    void setupTabbedViewContainer ();

    medReformatSelectorToolBox *reformatToolBox();

protected slots:
    void updateDataFromContainer();
    void updateUserLayerClosable(int tabIndex);
    void updateFromReformatSuccess(medAbstractData *output);
    //void addViewEventFilter(medViewEventFilter* filter);

private slots:
    void removeSelectorInternToolBox();

private:
    medReformatWorkspacePrivate *d;
};


