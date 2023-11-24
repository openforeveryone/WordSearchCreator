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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QActionGroup;

class WordSearchControl;
class WordSearchDrawer;
class WordSearchDoc;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WordSearchDoc *wsdoc =0, QWidget *parent = 0);
    ~MainWindow();

private:
    QDockWidget *wscdock;
    QDockWidget *dsdock;
    WordSearchDoc *wsdoc;
    WordSearchDrawer *wsdraw;
    WordSearchControl *wsc;
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *);
    void SetupWindow();
    QMenu *windowMenu;
    QList<QAction*> windowActions;
    QActionGroup *windowselectorGroup;
    QAction *thisWindowAction; //The action that represents this window.
    QList<QAction*> actionsDisableOnMinimise;
    bool event(QEvent *event);

private slots:
    void ShowDoc();
    void ShowAbout();
    void CheckUpdate();
    void download();
    void changelistorder(QAction *listorderaction);
    void changeWordSpace(QAction *action);
    void updateWindowMenu();
    void setWindowCheck();
    void windowSelected(QAction *action);
    void maximise();
    void minimise();
    void launchPreferences();
};

#endif
