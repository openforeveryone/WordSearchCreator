/***************************************************************************
 *   Copyright (C) 2006-2023 M Wellings                                    *
 *   info@openforeveryone.co.uk                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2 as     *
 *   published by the Free Software Foundation                             *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QMessageBox>
#include <QSplashScreen>
#include <QFileOpenEvent>
#ifdef Q_OS_WIN32
#include <QFontDatabase>
#endif

#include "wordsearchdoc.h"
#include "mainwindow.h"
#include "updatechecker.h"

#include "wordsearchapplication.h"

WordSearchApplication::WordSearchApplication( int & argc, char **argv ) :
    QApplication(argc,argv)
{
    QCoreApplication::setOrganizationName("OpenForEveryone");
    QCoreApplication::setOrganizationDomain("openforeveryone.co.uk");
    QCoreApplication::setApplicationName("Word Search Creator");
    Splash = new QSplashScreen(QPixmap(":/images/about.png"));
    Splash->show();
    processEvents();

    int fileloaded = false;
    WordSearchDoc *newwsd;
    //Are we being asked to open a file?
    if (QCoreApplication::arguments().size() > 1)
    {
        newwsd = new WordSearchDoc;
        QString fileName=QCoreApplication::arguments().last();
        //Open the file
        fileloaded = !newwsd->Open(fileName);
        if (!fileloaded)
            QMessageBox::warning(Splash,"Word Search Creator 1.2", QString("Cannot open file: %1").arg(fileName));
    }

    updateChecker = new UpdateChecker(this);
    updateChecker->checkForUpdate(false);

    if (!fileloaded)
    {
        //No file has been loaded in this function.
        //Bring up a blank window only if there are no other windows (that may have been created by a Mac asking us to open a file).
        if (windows.count()==0)
            window = new MainWindow();
        window->showMaximized();
        Splash->finish(window);
    }
    else
    {
        //A file was loaded in this function, show it in a new MainWindow.
        window = new MainWindow(newwsd);
        newwsd->setEditedState(false);
        window->showMaximized();
        Splash->finish(window);
    }
#ifdef Q_OS_WIN32
    //Make the fonts avalible to the application
    QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath().append("/URWGothicL-Book.ttf"));
#endif
}

void WordSearchApplication::loadFile(const QString &fileName)
{
    //This function is called when a Mac askes us to open a file.
    WordSearchDoc *newwsd;
    int fileloaded = false;
    newwsd = new WordSearchDoc;
    fileloaded = !newwsd->Open(fileName);
    if (!fileloaded)
        QMessageBox::warning(Splash,"Word Search Creator 1.2", QString("Cannot open file: %1").arg(fileName));
    window = new MainWindow(newwsd);
    newwsd->setEditedState(false);
    window->show();
}

bool WordSearchApplication::event(QEvent *event)
{
    //Catch any QEvent::FileOpen events and call loadFile(const QString &fileName) otherwise use the default behaviour.
    switch (event->type()) {
    case QEvent::FileOpen:
        loadFile(static_cast<QFileOpenEvent *>(event)->file());
        return true;
    default:
        return QApplication::event(event);
    }
}

void WordSearchApplication::registerWindow(MainWindow *window)
{
    windows.append(window);
    updateWindowLists();
}

void WordSearchApplication::deRegisterWindow(MainWindow *window)
{
    windows.removeAll(window);
    updateWindowLists();
}

void WordSearchApplication::updateWindowLists()
{
    //Emmiting this signal will cause all MainWindows to update their window menus.
    emit windowListChanged();
}

void WordSearchApplication::quitApplication()
{
    foreach (MainWindow *window, windows)
    {
        window->close();
    }
}
