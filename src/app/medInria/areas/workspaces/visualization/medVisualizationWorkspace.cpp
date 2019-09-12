/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2019. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medTabbedViewContainers.h>
#include <medVisualizationWorkspace.h>

void medVisualizationWorkspace::setupTabbedViewContainer()
{
    if (!tabbedViewContainers()->count())
    {
        this->tabbedViewContainers()->addContainerInTabNamed(this->name());
    }
    this->tabbedViewContainers()->unlockTabs();
}

bool medVisualizationWorkspace::isUsable()
{
    return true; // for the time being, no test is defined.
}
