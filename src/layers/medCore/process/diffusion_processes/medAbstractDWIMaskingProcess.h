#pragma once
/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2020. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractProcess.h>
#include <medCoreExport.h>
#include <dtkCore>

class medAbstractImageData;

class medAbstractDWIMaskingProcessPrivate;
class MEDCORE_EXPORT medAbstractDWIMaskingProcess : public medAbstractProcess
{
    Q_OBJECT

public:
    medAbstractDWIMaskingProcess(QObject *parent = nullptr);
    virtual ~medAbstractDWIMaskingProcess();

    virtual void setInput(medAbstractImageData* data);
    medAbstractImageData* input() const;
    medAbstractImageData* output() const;

protected:
    void setOutput(medAbstractImageData* data);
    virtual QString outputNameAddon() const {return "DWI mask";}

private:
    const QScopedPointer<medAbstractDWIMaskingProcessPrivate> d;
};

DTK_DECLARE_PLUGIN        (medAbstractDWIMaskingProcess, MEDCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(medAbstractDWIMaskingProcess, MEDCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(medAbstractDWIMaskingProcess, MEDCORE_EXPORT)
