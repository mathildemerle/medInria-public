#pragma once
/*
 * medInria
 * Copyright (c) INRIA 2013. All rights reserved.
 * 
 * medInria is under BSD-2-Clause license. See LICENSE.txt for details in the root of the sources or:
 * https://github.com/medInria/medInria-public/blob/master/LICENSE.txt
 * 
 * This software is distributed WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <QFrame>

#include <medNotif.h>
#include <medWidgetsExport.h>


class medNoti;
class medNotificationPaneWidget;

class QListWidgetItem;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QProgressBar;
class QGridLayout;


class medNotifWidget : public QFrame
{
    Q_OBJECT

public:
    medNotifWidget(medNotif *notif, medNotificationPaneWidget *paneContainer);
    ~medNotifWidget();

    QSize minimumSizeHint() const override;

public slots:
    void update(medNotif * notif);

private:
    void extraTriggerUpdate(int extraCount, medNotif * notif);
    static QPixmap & criticalityImg(notifLevel criticalityLevel);

private:
    QHBoxLayout  * m_titleLayout;
    QLabel       * m_titleLabel;

    QLabel       * m_imageLabel;
    QPushButton  * m_removeNotifButton;

    QLabel       * m_msgLabel;
    QProgressBar * m_progressWidget;

    QGridLayout  * m_extraLayout;
    QPushButton  * m_extraTrigger1Button;
    QPushButton  * m_extraTrigger2Button;
    QPushButton  * m_extraTrigger3Button;

    QSize          m_sourceWidgetSize;
};