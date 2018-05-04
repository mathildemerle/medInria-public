/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QObject>

#include <medCoreExport.h>



class medIntParameter;

class medAbstractJobPrivate;
class MEDCORE_EXPORT medAbstractJob: public QObject
{
    Q_OBJECT

public:
    enum medJobExitStatus
    {
        MED_JOB_EXIT_FAILURE,
        MED_JOB_EXIT_CANCELLED,
        MED_JOB_EXIT_SUCCESS,

        //MUSIC: exit status which will display error message. Cf. medToolBox::handleDisplayError
        PIXEL_TYPE,     //! Pixel type not yet implemented
        DIMENSION_3D,   //! Not a 3D volume
        DIMENSION_4D,   //! Not a 4D volume
        MESH_TYPE,      //! Not a mesh
        NO_MESH,        //! Input can not be a mesh
        DATA_SIZE,      //! Inputs must be the same size
        MISMATCHED_DATA_TYPES,                //! Inputs must be the same type
        MISMATCHED_DATA_SIZES_ORIGIN_SPACING, //! Inputs should have the same size, origin, spacing
        MISMATCHED_DATA_SIZE,    //! Inputs should have the same size
        MISMATCHED_DATA_ORIGIN,  //! Inputs should have the same origin
        MISMATCHED_DATA_SPACING, //! Inputs should have the same spacing
        UNDEFINED,               //! Miscellanous
    };
    Q_ENUMS(medJobExitStatus)


    medAbstractJob(QObject *parent = NULL);
    virtual ~medAbstractJob();

    virtual QString caption() const = 0;

public:
    virtual medJobExitStatus run() = 0;
    virtual void cancel() = 0;
    bool isRunning() const;

    medIntParameter* progression() const;

signals:
    void finished(medJobExitStatus exitStatus);

private slots:
    void _setIsRunning(bool isRunning);
    void _resetProgression();

signals:
    void running(bool isRunning);

private:
    const QScopedPointer<medAbstractJobPrivate> d;

};
