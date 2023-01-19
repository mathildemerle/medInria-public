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
#include <QPushButton>
#include <QTreeWidget>

#include <medSourcesSettings.h>
#include <medWidgetsExport.h>


 /**
  * @class   medSourcesSettingsHandlerWidget
  * @author  Florent Leray
  * @date    27/04/2022
  *
  * @brief   medSourcesSettingsHandlerWidget is the sub-widget to display and handle current selected source instance inside the m_sourceListWidget.
  * This widget allows:
  *   - status displaying.
  *   - set current source as the default.
  *   - remove the current source.
  */
class MEDWIDGETS_EXPORT medSourcesSettingsHandlerWidget : public QWidget
{
    Q_OBJECT

public:
    medSourcesSettingsHandlerWidget(medSourcesSettings *pi_parent = nullptr);
    ~medSourcesSettingsHandlerWidget() = default;

    void sourceChange(medAbstractSource * pi_pSource, bool p_default);

  public slots:
    void setAsDefault();

protected:
    void updateConnectButton(medAbstractSource * pi_pSource);
    void updateConnectButtonText(bool pi_bConnect);
    void updateSourceInformation(medAbstractSource * pi_pSource);

private:
    QMetaObject::Connection m_qtConnections[2]; /*!< Store internal connections to provide a simple and clear way to disconnect when source change. */

    QPushButton * m_setDefaultButton;  /*!< Widget button to define the default source. */
    QPushButton * m_connectButton;     /*!< Widget button to connect and disconnect source. */
    QTreeWidget * m_sourceInformation; /*!< Widget display source status. */
    QPushButton * m_removeButton;      /*!< Widget button to remove the current source. */
};
