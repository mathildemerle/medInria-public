/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtCore>

class medAbstractView;
class medAbstractLayeredView;
class medParameterPool;
class medAbstractParameterGroupPrivate;

class medAbstractParameterGroup : public QObject
{
public:
    medAbstractParameterGroup(QString name = "", QObject *parent = 0);
    virtual ~medAbstractParameterGroup();

    void setName(QString name);
    QString name() const;

    void addParameterToLink(QString parameter);
    void removeParameter(QString parameter);

    void setParametersToLink(QStringList parameters);
    QStringList parameters() const;

    void saveAsPreset();

    virtual void update() = 0;

private:
    medAbstractParameterGroupPrivate *d;
};

