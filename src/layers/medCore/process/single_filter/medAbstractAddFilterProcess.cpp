/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medDoubleParameter.h>
#include <medAbstractAddFilterProcess.h>

medAbstractAddFilterProcess::medAbstractAddFilterProcess(QObject *parent)
    : medAbstractSingleFilterOperationDoubleProcess(parent)
{
    this->doubleParameter()->setCaption("Added constant");
    this->doubleParameter()->setDescription("Value of added constant");
}