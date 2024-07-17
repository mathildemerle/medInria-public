/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.

 See LICENSE.txt for details in the root of the sources or:
 https://github.com/medInria/medInria-public/blob/master/LICENSE.txt

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.

=========================================================================*/

#include "medApplication.h"

#include <medAbstractDataFactory.h>
#include <medCore.h>
#include <medDatabaseSettingsWidget.h>
#include <medDevelopmentSettingsWidget.h>
#include <medDataManager.h>
#include <medDiffusionWorkspace.h>
#include <medFilteringWorkspace.h>
#include <medLogger.h>
#include <medMainWindow.h>
#include <medPluginManager.h>
#include <medSeedPointAnnotationData.h>
#include <medSettingsManager.h>
#include <medSettingsWidgetFactory.h>
#include <medStartupSettingsWidget.h>
#include <medStyleSheetParser.h>
#include <medVisualizationWorkspace.h>
#include <medWorkspaceFactory.h>

class medApplicationPrivate
{
public:
    medMainWindow *mainWindow;
    QStringList systemOpenInstructions;
    QSplashScreen *splashScreen;
};

// /////////////////////////////////////////////////////////////////
// medApplication
// /////////////////////////////////////////////////////////////////

medApplication::medApplication(int & argc, char**argv) :
    QtSingleApplication(argc,argv),
    d(new medApplicationPrivate)
{
    d->mainWindow = nullptr;

    // Themes
    QVariant themeChosen = medSettingsManager::instance().value("startup","theme");
    int themeIndex = themeChosen.toInt();
    QPixmap splashLogo;
    switch (themeIndex)
    {
        case 0:
        case 1:
        case 2:
        default:
        {
            splashLogo.load(":MUSICardio-2023-lightfont-notext-darkback-margin10.png");
            break;
        }
        case 3:
        case 4:
        {
            splashLogo.load(":MUSICardio-2023-darkfont-notext-whiteback-margin10.png");
            break;
        }
    }
    splashLogo = splashLogo.scaled(719, 87, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    d->splashScreen = new QSplashScreen(splashLogo,
                                        Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    d->splashScreen->setAttribute(Qt::WA_DeleteOnClose, true);
    d->splashScreen->show();
    this->processEvents();

    this->setApplicationName(PROJECT_NAME); /*Beware, change database path*/
    this->setApplicationVersion(MEDINRIA_VERSION);
    this->setOrganizationName("INRIA_IHU-LIRYC"); /*Beware, change database path*/
    this->setOrganizationDomain("fr");

    medLogger::initialize();

    qInfo() << "####################################";
    qInfo() << "Version: "    << MEDINRIA_VERSION;
    qInfo() << "Build Date: " << MEDINRIA_BUILD_DATE;

    QApplication::setStyle(QStyleFactory::create("fusion"));

    // Expiration Date
    QDate expiryDate = QDate::fromString(QString(MEDINRIA_BUILD_DATE), "dd_MM_yyyy")
                                        .addMonths(EXPIRATION_TIME);
    qInfo() << "Expiration Date: " << qPrintable(expiryDate.toString());

    if ( ! expiryDate.isValid() || QDate::currentDate() > expiryDate)
    {
        QString expiredInfo = "This copy of ";
        expiredInfo += (char*)(PROJECT_NAME);
        expiredInfo += " has expired, please contact ";
        expiredInfo += (char*)(PROJECT_CONTACT);
        expiredInfo += " for more information.";
        QMessageBox msg;
        msg.setText(expiredInfo);
        msg.exec();
        ::exit(1);
    }

    QString qssFile;
    switch (themeIndex)
    {
    case 0:
    default:
        // Dark Grey
        qssFile = ":/music_darkGrey.qss";
        break;
    case 1:
        // Dark Blue
        qssFile = ":/music_dark.qss";
        break;
    case 2:
        // medInria
        qssFile = ":/medInria.qss";
        break;
    case 3:
        // Light Grey
        qssFile = ":/music_lightGrey.qss";
        break;
    case 4:
        // Light
        qssFile = ":/music_light.qss";
        break;
    }
    this->setWindowIcon(QIcon(":MUSICardio_logo_small_light.png"));
    medStyleSheetParser parser(dtkReadFile(qssFile));
    this->setStyleSheet(parser.result());

    this->initialize();
}

medApplication::~medApplication(void)
{
    delete d;
    d = nullptr;
}

bool medApplication::event(QEvent *event)
{
    switch (event->type())
    {
        // Handle file system open requests, but only if the main window has been created and set
        case QEvent::FileOpen:
            if (d->mainWindow)
                emit messageReceived(QString("/open ") + static_cast<QFileOpenEvent *>(event)->file());
            else
                d->systemOpenInstructions.append(QString("/open ") + static_cast<QFileOpenEvent *>(event)->file());

            return true;
        default:
            return QtSingleApplication::event(event);
    }
}

void medApplication::setMainWindow(medMainWindow *mw)
{
    d->mainWindow = mw;

    QVariant var = QVariant::fromValue((QObject*)d->mainWindow);
    this->setProperty("MainWindow",var);
    d->systemOpenInstructions.clear();

    // Wait until the app is displayed to close itself
    d->splashScreen->finish(d->mainWindow); 
}

void medApplication::redirectMessageToSplash(const QString &message)
{
    emit showMessage(message);
}

void medApplication::open(const medDataIndex & index)
{
    d->mainWindow->open(index);
}

void medApplication::open(QString path)
{
    d->mainWindow->open(path);
}

void medApplication::initialize()
{
    qRegisterMetaType<medDataIndex>("medDataIndex");

    // Registering different workspaces
    medWorkspaceFactory * viewerWSpaceFactory = medWorkspaceFactory::instance();
    viewerWSpaceFactory->registerWorkspace<medVisualizationWorkspace>();
    viewerWSpaceFactory->registerWorkspace<medDiffusionWorkspace>();
    viewerWSpaceFactory->registerWorkspace<medFilteringWorkspace>();

    //Register settingsWidgets
    medSettingsWidgetFactory* settingsWidgetFactory = medSettingsWidgetFactory::instance();
    settingsWidgetFactory->registerSettingsWidget<medStartupSettingsWidget>();
    settingsWidgetFactory->registerSettingsWidget<medDatabaseSettingsWidget>();
    settingsWidgetFactory->registerSettingsWidget<medDevelopmentSettingsWidget>();

    //Register annotations
    medAbstractDataFactory * datafactory = medAbstractDataFactory::instance();
    datafactory->registerDataType<medSeedPointAnnotationData>();

    // process layer:
    QString pluginsPath = getenv("MEDINRIA_PLUGINS_DIR");
    QString defaultPath;
    QDir plugins_dir;
#ifdef Q_OS_MAC
    plugins_dir = qApp->applicationDirPath() + "/../PlugIns";
#elif defined(Q_OS_WIN)
    plugins_dir = qApp->applicationDirPath() + "/plugins";
#else
    plugins_dir = qApp->applicationDirPath() + "/plugins";
#endif
    defaultPath = plugins_dir.absolutePath();

    if ( !pluginsPath.isEmpty() )
        medCore::pluginManager::initialize(pluginsPath);
    else
        medCore::pluginManager::initialize(defaultPath);
}
