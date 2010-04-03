/*=====================================================================

PIXHAWK Micro Air Vehicle Flying Robotics Toolkit

(c) 2009, 2010 PIXHAWK PROJECT  <http://pixhawk.ethz.ch>

This file is part of the PIXHAWK project

    PIXHAWK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PIXHAWK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PIXHAWK. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Main class
 *
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *
 */


#include <QFile>
#include <QFlags>
#include <QThread>
#include <QSplashScreen>
#include <QPixmap>
#include <QDesktopWidget>
#include <QPainter>
#include <QStyleFactory>
#include <QAction>

#include <Core.h>
#include <MG.h>
#include <MainWindow.h>
#include "AudioOutput.h"


/**
 * @brief Constructor for the main application.
 *
 * This constructor initializes and starts the whole application. It takes standard
 * command-line parameters
 *
 * @param argc The number of command-line parameters
 * @param argv The string array of parameters
 **/

MGCore::MGCore(int &argc, char* argv[]) : QApplication(argc, argv)
{
    this->setApplicationName("OpenMAV Ground Control Station");
    this->setApplicationVersion("v. 0.0.5");
    this->setOrganizationName(QLatin1String("OpenMAV Association"));
    QSettings::setDefaultFormat(QSettings::IniFormat);
    // Exit main application when last window is closed
    connect(this, SIGNAL(lastWindowClosed()), this, SLOT(quit()));

    // Set application font
    QFontDatabase fontDatabase = QFontDatabase();
    const QString fontFileName = ":/general/vera.ttf"; ///< Font file is part of the QRC file and compiled into the app
    const QString fontFamilyName = "Bitstream Vera Sans";
    if(!QFile::exists(fontFileName)) printf("ERROR! font file: %s DOES NOT EXIST!", fontFileName);
    fontDatabase.addApplicationFont(fontFileName);
    setFont(fontDatabase.font(fontFamilyName, "Roman", 12));

    // Show splash screen
    QPixmap splashImage(MG::DIR::getIconDirectory() + "/groundstation-splash.png");
    QSplashScreen* splashScreen = new QSplashScreen(splashImage, Qt::WindowStaysOnTopHint);
    splashScreen->show();

    // Start the comm link manager
    splashScreen->showMessage(tr("Starting Communication Links"));
    startLinkManager();

    // Start the UAS Manager
    splashScreen->showMessage(tr("Starting UAS Manager"));
    startUASManager();

    // Start audio output
    AudioOutput* audio = new AudioOutput();
    audio->say("Ground Control Station started", 1);

    tarsus = new ViconTarsusProtocol();
    tarsus->start();

    // Start the user interface
    splashScreen->showMessage(tr("Starting User Interface"));
    startUI();

    // Remove splash screen
    splashScreen->finish(mainWindow);


}

/**
 * @brief Destructor for the groundstation. It destroys all loaded instances.
 *
 **/
MGCore::~MGCore()
{
    // Delete singletons
    delete LinkManager::instance();
    delete UASManager::instance();
}

/**
 * @brief Start the link managing component.
 *
 * The link manager keeps track of all communication links and provides the global
 * packet queue. It is the main communication hub
 **/
void MGCore::startLinkManager()
{
    LinkManager::instance();
}

/**
 * @brief Start the Unmanned Air System Manager
 *
 **/
void MGCore::startUASManager()
{
    UASManager::instance();
}

/**
 * @brief Start and show the user interface.
 **/
void MGCore::startUI()
{
    // Start UI
    mainWindow = new MGMainWindow();
    // Make UI visible
    mainWindow->show();
}

