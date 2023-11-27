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

#ifndef WORDSEARCHAPPLICATION_H
#define WORDSEARCHAPPLICATION_H

#include <QApplication>

class MainWindow;
class UpdateChecker;
class QSplashScreen;
class QMenu;
class QAction;
class QActionGroup;

class WordSearchApplication : public QApplication
{
    Q_OBJECT
public:
    explicit WordSearchApplication( int & argc, char **argv );
    bool event(QEvent *event);
    void registerWindow(MainWindow *window);
    void deRegisterWindow(MainWindow *window);
    QList<MainWindow*> windows;
    UpdateChecker *updateChecker;
#ifdef Q_OS_MAC
    QMenu *dockMenu = 0;
#endif
    void setWindowCheck();

private:
    MainWindow *window;
    void loadFile(const QString &fileName);
    QSplashScreen *Splash ;
#ifdef Q_OS_MAC
    QList<QAction*> windowActions;
    QActionGroup *windowselectorGroup;
    QAction *dockSerprator;
    void updateDockMenu();
    void setupDockMenu();
#endif

signals:
    void windowListChanged();
    void currentWindowChanged();

public slots:
    void quitApplication();
    void updateWindowLists();    
    void New();

private slots:
    void windowSelected(QAction *action);
};

#endif // WORDSEARCHAPPLICATION_H
