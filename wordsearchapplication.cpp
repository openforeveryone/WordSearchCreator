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
#include <QMenu>
#ifdef Q_OS_WIN32
#include <QFontDatabase>
#endif

#include <QDebug>

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

    dockMenu = new QMenu("DockMenu");
    dockMenu->setAsDockMenu();
    dockSerprator = dockMenu->addSeparator();
    QAction *newAct = new QAction(tr("New Wordsearch"), this);
    dockMenu->addAction(newAct);
    connect(newAct, SIGNAL(triggered()), this, SLOT(New()));
    windowselectorGroup = new QActionGroup(this);
    connect(windowselectorGroup, SIGNAL(triggered(QAction*)), this, SLOT(windowSelected(QAction*)));

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
    //Emitting this signal will cause all MainWindows to update
    //their window menus and the dock menu to be updated.
    emit windowListChanged();
    updateDockMenu();
}

void WordSearchApplication::quitApplication()
{
    foreach (MainWindow *window, windows)
    {
        window->close();
    }
}

void WordSearchApplication::updateDockMenu()
{
    qDebug() << "updateDockMenu()";
    foreach (QAction *action, windowActions)
    {
        dockMenu->removeAction(action);
        delete action;
    }
    windowActions.clear();
    int i=0;
    foreach (MainWindow *window, windows)
    {
        QString title = window->windowTitle();
        //Remove the edited placeholder. Shoud be done better than this.
        title.remove("[*]");
        QAction *windowAction = new QAction(title, this);
        dockMenu->insertAction(dockSerprator, windowAction);
        windowAction->setCheckable(true);
        windowAction->setData(i);
        windowAction->setActionGroup(windowselectorGroup);
        if (window->isActiveWindow())
        {
            windowAction->setChecked(true);
        }
        windowActions.append(windowAction);
        i++;
    }
}

void WordSearchApplication::setWindowCheck()
{
    qDebug() << "setWindowCheck()";
    int i=0;
    foreach (MainWindow *window, windows)
    {
        windowActions[i]->setChecked(window->isActiveWindow());
        i++;
    }
    emit currentWindowChanged();
}

void WordSearchApplication::windowSelected(QAction *action)
{
    //One of the windows was selected off the dock menu
    QMainWindow *selctedWindow = windows.at(action->data().toInt());
    qDebug() << "window Selected " << action->data().toInt() << " " << selctedWindow->windowTitle() << "State: " << selctedWindow->windowState();
    if (selctedWindow->windowState().testFlag(Qt::WindowState::WindowMinimized))
    {
        //Seems the only way to restore a minimised window on macOS is to use showMaximized() or showNormal() but we must know which to use.
        if(selctedWindow->windowState().testFlag(Qt::WindowState::WindowMaximized))
        {
            selctedWindow->showMaximized();
        }
        else
        {
            selctedWindow->showNormal();
        }
    }
    else
    {
        qDebug() << "Raising Window";
        selctedWindow->raise();
        selctedWindow->activateWindow();
    }
}

void WordSearchApplication::New()
{
    MainWindow *window = new MainWindow();
    window->show();
}
