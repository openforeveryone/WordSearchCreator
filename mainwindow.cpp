/***************************************************************************
 *   Copyright (C) 2006-2024 M Wellings                                    *
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

#include <QScrollArea>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QActionGroup>
#include <QDesktopServices>
#include <QCloseEvent>

#include "wordsearchdrawer.h"
#include "wordsearchdoc.h"
#include "wordsearchcontrol.h"
#include "maskdrawer.h"
#include "wordsearch.h"
#include "wordsearchapplication.h"
#include "updatechecker.h"
#include "preferencesdialog.h"

#include "mainwindow.h"

MainWindow::MainWindow(WordSearchDoc *wsdoc, QWidget *parent)
    : QMainWindow(parent)
{
    wsc = new WordSearchControl;
    if (wsdoc!=0)
        this->wsdoc = wsdoc;
    else
        this->wsdoc = new WordSearchDoc;
//    connect(wsdoc, SIGNAL(EditedStateChanged(bool), this, SLOT())
    wsdraw = new WordSearchDrawer(this->wsdoc);
    SetupWindow();
    wsc->setWordSearchDoc(this->wsdoc);
    if (wsdoc==0)
        wsc->SetupTemplate();
    uint i = 0;
    while (i < this->wsdoc->ws->wordlist.size())
    {
        if (this->wsdoc->ws->wordlist[i].used!=0 && this->wsdoc->ws->wordlist[i].answered==true)
            clearPlayAnswersAction->setEnabled(true);
        i++;
    }
}

void MainWindow::SetupWindow()
{
    setWindowFilePath(QString("Untitled"));
#ifndef Q_OS_MAC
    setWindowTitle("Untitled[*] - Word Search Creator");
#endif
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(wsdraw);

    scrollArea->setWidgetResizable(true);
    this->setCentralWidget(scrollArea);
    setWindowState(Qt::WindowMaximized);
    wscdock = new QDockWidget("Control Box", this);
    wscdock->setWidget(wsc);
    wscdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    wscdock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    addDockWidget(Qt::RightDockWidgetArea, wscdock);

    QAction *titleFontAct = new QAction(tr("Set &Title Font..."), this);
    titleFontAct->setShortcut(tr("Ctrl+T"));
    QAction *PrintAct = new QAction(tr("Print Worksheet..."), this);
    PrintAct->setShortcut(tr("Ctrl+P"));
    PrintAct->setStatusTip(tr("Print Worksheet..."));
    PrintAct->setIcon(QIcon(":/icons/fileprint.png"));
    PrintAct->setIconText("Print");
    connect(PrintAct, SIGNAL(triggered()), wsdraw, SLOT(printWithDialog()));
    QAction *DPrintAct = new QAction(tr("Print Worksheet Directly..."), this);
    DPrintAct->setStatusTip(tr("Print Worksheet Directly"));
    DPrintAct->setIcon(QIcon(":/icons/fileprint.png"));
    DPrintAct->setIconText("Print");
    connect(DPrintAct, SIGNAL(triggered()), wsdraw, SLOT(printDirect()));
    QAction *quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcut(tr("Ctrl+Q"));
    quitAct->setStatusTip(tr("Quit"));
    quitAct->setIcon(QIcon(":/icons/exit.png"));
    connect(quitAct, SIGNAL(triggered()), QApplication::instance(), SLOT(quitApplication()));
    QAction *closeAct = new QAction(tr("&Close"), this);
    closeAct->setShortcut(tr("Ctrl+W"));
    closeAct->setStatusTip(tr("Quit"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));
    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save Wordsearch"));
    saveAct->setIcon(QIcon(":/icons/filesave.png"));
    connect(wsdoc, SIGNAL(EditedStateChanged(bool)), saveAct, SLOT(setEnabled(bool)));
    connect(saveAct, SIGNAL(triggered()), wsc, SLOT(Save()));
    QAction *PDFAct = new QAction(tr("&Export as PDF..."), this);
    PDFAct->setIcon(QIcon(":/icons/exportpdf.png"));
    connect(PDFAct, SIGNAL(triggered()), wsdraw, SLOT(PDF()));
    QAction *SVGAct = new QAction(tr("&Export as SVG..."), this);
    connect(SVGAct, SIGNAL(triggered()), wsdraw, SLOT(SVG()));
    QAction *saveAsAct = new QAction(tr("&Save As..."), this);
    connect(saveAsAct, SIGNAL(triggered()), wsc, SLOT(SaveAs()));
    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open Wordsearch"));
    openAct->setIcon(QIcon(":/icons/fileopen.png"));
    connect(openAct, SIGNAL(triggered()), wsc, SLOT(Open()));
    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("New Wordsearch"));
    newAct->setIcon(QIcon(":/icons/filenew.png"));
    connect(newAct, SIGNAL(triggered()), wsc, SLOT(New()));
//    QAction *upAct = new QAction(tr("&Upload Word Search..."), this);
//    upAct->setShortcut(tr("Ctrl+U"));
//    upAct->setStatusTip(tr("Share your word search online"));
//    connect(upAct, SIGNAL(triggered()), wsc, SLOT(upload()));
    QAction *downAct = new QAction(tr("&Download More..."), this);
    downAct->setStatusTip(tr("Find more word searches online"));
    connect(downAct, SIGNAL(triggered()), this, SLOT(download()));
    QAction *prefAct = new QAction(tr("&Preferences..."), this);
    connect(prefAct, SIGNAL(triggered()), this, SLOT(launchPreferences()));
    QAction *appearanceAct = new QAction(tr("Text Font, Size and Colours..."), this);
    appearanceAct->setIcon(QIcon(":/icons/text.png"));
    connect(appearanceAct, SIGNAL(triggered()), wsc, SLOT(appearance()));
    QAction *dsAct = new QAction(tr("Set allowed directions..."), this);
    connect(dsAct, SIGNAL(triggered()), wsc, SLOT(showdirectionselector()));
    QAction *mdAct = new QAction(tr("Change grid shape..."), this);
    connect(mdAct, SIGNAL(triggered()), wsc, SLOT(showmaskdrawer()));
    QAction *docAct = new QAction(tr("Help Contents"), this);
    docAct->setIcon(QIcon(":/icons/contents.png"));
    connect(docAct, SIGNAL(triggered()), this, SLOT(ShowDoc()));
    QAction *checkAct = new QAction(tr("Check For Updates"), this);
    checkAct->setMenuRole(QAction::ApplicationSpecificRole);
    connect(checkAct, SIGNAL(triggered()), this, SLOT(CheckUpdate()));
    QAction *aboutAct = new QAction(tr("About Word Search Creator"), this);
    aboutAct->setIcon(QIcon(":/icons/22x22icon.png"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(ShowAbout()));
    QAction *listAct = new QAction(tr("Word List Creator..."), this);
    //aboutAct->setIcon(QIcon(":/icons/22x22icon.png"));
    connect(listAct, SIGNAL(triggered()), wsc, SLOT(showlistCreator()));
    QAction *zoomIn = new QAction(tr("Zoom In"), this);
    zoomIn->setIcon(QIcon(":/icons/zoomin.png"));
    zoomIn->setShortcut(tr("Ctrl++"));
    connect(zoomIn, SIGNAL(triggered()), wsdraw, SLOT(zoomIn()));
    QAction *zoomOut = new QAction(tr("Zoom Out"), this);
    zoomOut->setIcon(QIcon(":/icons/zoomout.png"));
    zoomOut->setShortcut(tr("Ctrl+-"));
    connect(zoomOut, SIGNAL(triggered()), wsdraw, SLOT(zoomOut()));
    QAction *autoZoom = new QAction(tr("Zoom page width"), this);
    connect(autoZoom, SIGNAL(triggered()), wsdraw, SLOT(autoZoom()));
    QActionGroup *layoutGroup = new QActionGroup(this);
    QAction *pageLayout = new QAction(tr("Page View"), this);
    pageLayout->setCheckable(true);
    pageLayout->setActionGroup(layoutGroup);
    QAction *screenLayout = new QAction(tr("Screen View"), this);
    screenLayout->setCheckable(true);
    screenLayout->setActionGroup(layoutGroup);
    pageLayout->setChecked(true);
    connect(pageLayout, SIGNAL(triggered()), wsdraw, SLOT(pageLayout()));
    connect(screenLayout, SIGNAL(triggered()), wsdraw, SLOT(screenLayout()));
    QAction *setFooter = new QAction(tr("Edit Footer..."), this);
    connect(setFooter, SIGNAL(triggered()), wsc, SLOT(setFooter()));
    QAction *showCB = new QAction(tr("Show Control Box"), this);
    showCB->setCheckable(true);
    showCB->setChecked(true);
    connect(showCB, SIGNAL(toggled(bool)), wscdock, SLOT(setVisible(bool)));
    connect(wscdock, SIGNAL(visibilityChanged (bool)), showCB, SLOT(setChecked(bool)));
    QAction *pageLayoutd = new QAction(tr("Page Layout..."), this);
    connect(pageLayoutd, SIGNAL(triggered()), wsc, SLOT(showpageLayout()));
    QAction *copy = new QAction(tr("Copy as image"), this);
    copy->setShortcut(tr("Ctrl+c"));
    connect(copy, SIGNAL(triggered()), wsdraw, SLOT(Copy()));
    //QAction *allowplay = new QAction(tr("Play on screen"), this);
    //allowplay->setCheckable(true);
    //allowplay->setChecked(true);
    //connect(allowplay, SIGNAL(toggled(bool)), wsdraw, SLOT(setAllowPlay(bool)));
    clearPlayAnswersAction = new QAction(tr("Clear answers"), this);
    clearPlayAnswersAction->setEnabled(false);
    connect(clearPlayAnswersAction, SIGNAL(triggered()), wsdraw, SLOT(clearPlayAnswers()));
    connect(wsdraw, SIGNAL(someAnswerdChanged(bool)), clearPlayAnswersAction, SLOT(setEnabled(bool)));
    QAction *alphabetAct = new QAction(tr("Fill Alphabet..."), this);
    connect(alphabetAct, SIGNAL(triggered()), wsc, SLOT(alphabet()));
    QActionGroup *listorderGroup = new QActionGroup(this);
    connect(listorderGroup, SIGNAL(triggered(QAction*)), this, SLOT(changelistorder(QAction*)));
    QAction *listorderAOAct = new QAction(tr("Original order"), this);
    listorderAOAct->setCheckable(true);
    listorderAOAct->setActionGroup(listorderGroup);
    listorderAOAct->setData(0);    
    if (wsdoc->wordlistorder == WordSearchDoc::UserDefined)
        listorderAOAct->setChecked(true);
    QAction *listorderAAAct = new QAction(tr("Alphabetical"), this);
    listorderAAAct->setCheckable(true);
    listorderAAAct->setActionGroup(listorderGroup);
    listorderAAAct->setData(1);
    if (wsdoc->wordlistorder == WordSearchDoc::Alphabetical)
        listorderAAAct->setChecked(true);
    QAction *listorderADAct = new QAction(tr("Reverse Alphabetical"), this);
    listorderADAct->setCheckable(true);
    listorderADAct->setActionGroup(listorderGroup);
    listorderADAct->setData(2);
    if (wsdoc->wordlistorder == WordSearchDoc::ReverseAlphabetical)
        listorderADAct->setChecked(true);
    QAction *listorderLDAct = new QAction(tr("Word Length (Des)"), this);
    listorderLDAct->setCheckable(true);
    listorderLDAct->setActionGroup(listorderGroup);
    listorderLDAct->setData(3);
    if (wsdoc->wordlistorder == WordSearchDoc::SizeDes)
        listorderLDAct->setChecked(true);
    QAction *listorderLAAct = new QAction(tr("Word Length (Asc)"), this);
    listorderLAAct->setCheckable(true);
    listorderLAAct->setActionGroup(listorderGroup);
    listorderLAAct->setData(4);
    if (wsdoc->wordlistorder == WordSearchDoc::SizeAsc)
        listorderLAAct->setChecked(true);
    QActionGroup *wordSpaceGroup = new QActionGroup(this);
    connect(wordSpaceGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeWordSpace(QAction*)));
    QAction *wordSpaceFSAct = new QAction(tr("Fill Space"), this);
    wordSpaceFSAct->setCheckable(true);
    wordSpaceFSAct->setActionGroup(wordSpaceGroup);
    wordSpaceFSAct->setData(0);
    if (wsdoc->ws->wordSpace == wordsearch::FillSpace)
        wordSpaceFSAct->setChecked(true);
    QAction *wordSpaceRSAct = new QAction(tr("Remove Space"), this);
    wordSpaceRSAct->setCheckable(true);
    wordSpaceRSAct->setActionGroup(wordSpaceGroup);
    wordSpaceRSAct->setData(1);
    if (wsdoc->ws->wordSpace == wordsearch::RemoveSpace)
        wordSpaceRSAct->setChecked(true);
    QAction *wordSpaceKSAct = new QAction(tr("Keep Space"), this);
    wordSpaceKSAct->setCheckable(true);
    wordSpaceKSAct->setActionGroup(wordSpaceGroup);
    wordSpaceKSAct->setData(2);
    if (wsdoc->ws->wordSpace == wordsearch::KeepSpace)
        wordSpaceKSAct->setChecked(true);
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QMenu *formatMenu = menuBar()->addMenu(tr("&Format"));
    QMenu *wordsearchMenu = menuBar()->addMenu(tr("Word Search"));
    QMenu *wordlistMenu = menuBar()->addMenu(tr("Word List"));

#ifdef Q_OS_MAC
	//Hide icons in menus:
	newAct->setIconVisibleInMenu(false);
	saveAct->setIconVisibleInMenu(false);
	saveAsAct->setIconVisibleInMenu(false);
	openAct->setIconVisibleInMenu(false);
	PDFAct->setIconVisibleInMenu(false);
	PrintAct->setIconVisibleInMenu(false);
    DPrintAct->setIconVisibleInMenu(false);
    quitAct->setIconVisibleInMenu(false);
	closeAct->setIconVisibleInMenu(false);
	saveAct->setIconVisibleInMenu(false);
	appearanceAct->setIconVisibleInMenu(false);
	zoomIn->setIconVisibleInMenu(false);
	zoomOut->setIconVisibleInMenu(false);
	docAct->setIconVisibleInMenu(false);
    aboutAct->setIconVisibleInMenu(false);

    //Mac window menu:
    windowMenu = menuBar()->addMenu(tr("&Window"));
    QAction *minimise = new QAction(tr("Minimise"), this);
    connect(minimise, SIGNAL(triggered()), this, SLOT(minimise()));
    minimise->setShortcut(tr("Ctrl+M"));
    windowMenu->addAction(minimise);
    QAction *maximise = new QAction(tr("Zoom"), this);
    connect(maximise, SIGNAL(triggered()), this, SLOT(maximise()));
    windowMenu->addAction(maximise);
    windowMenu->addSeparator();
    windowselectorGroup = new QActionGroup(this);
    connect(windowselectorGroup, SIGNAL(triggered(QAction*)), this, SLOT(windowSelected(QAction*)));

    //These actions need dissabling when the window is minimised
    actionsDisableOnMinimise.append(closeAct);
    actionsDisableOnMinimise.append(saveAct);
    actionsDisableOnMinimise.append(saveAsAct);
    actionsDisableOnMinimise.append(PDFAct);
    actionsDisableOnMinimise.append(SVGAct);
    actionsDisableOnMinimise.append(PrintAct);
    actionsDisableOnMinimise.append(zoomIn);
    actionsDisableOnMinimise.append(zoomOut);
    actionsDisableOnMinimise.append(autoZoom);
    actionsDisableOnMinimise.append(pageLayout);
    actionsDisableOnMinimise.append(screenLayout);
    actionsDisableOnMinimise.append(showCB);
    actionsDisableOnMinimise.append(appearanceAct);
    actionsDisableOnMinimise.append(pageLayoutd);
    actionsDisableOnMinimise.append(setFooter);
    actionsDisableOnMinimise.append(dsAct);
    actionsDisableOnMinimise.append(mdAct);
    actionsDisableOnMinimise.append(copy);
    actionsDisableOnMinimise.append(alphabetAct);
    actionsDisableOnMinimise.append(wordSpaceFSAct);
    actionsDisableOnMinimise.append(wordSpaceRSAct);
    actionsDisableOnMinimise.append(wordSpaceKSAct);
    actionsDisableOnMinimise.append(listAct);
    actionsDisableOnMinimise.append(listorderAOAct);
    actionsDisableOnMinimise.append(listorderAAAct);
    actionsDisableOnMinimise.append(listorderADAct);
    actionsDisableOnMinimise.append(listorderLDAct);
    actionsDisableOnMinimise.append(listorderLAAct);
    actionsDisableOnMinimise.append(minimise);
    actionsDisableOnMinimise.append(maximise);
#endif

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(PDFAct);
    fileMenu->addAction(SVGAct);
    fileMenu->addSeparator();
//    fileMenu->addAction(upAct);
    fileMenu->addAction(downAct);
    fileMenu->addSeparator();
    fileMenu->addAction(prefAct);
    fileMenu->addSeparator();
    fileMenu->addAction(PrintAct);
    fileMenu->addSeparator();
    //We don't want the quit action on mac as it overides the default behavior (which works well).
//#ifndef Q_WS_MAC
    //fileMenu->addAction(quitAct);
//#endif
    viewMenu->addAction(zoomIn);
    viewMenu->addAction(zoomOut);
    viewMenu->addAction(autoZoom);
    viewMenu->addSeparator();
    viewMenu->addAction(pageLayout);
    viewMenu->addAction(screenLayout);
    viewMenu->addSeparator();
    viewMenu->addAction(showCB);
    formatMenu->addAction(appearanceAct);
    formatMenu->addAction(pageLayoutd);
    formatMenu->addAction(setFooter);
    wordsearchMenu->addAction(dsAct);
    wordsearchMenu->addAction(mdAct);
    wordsearchMenu->addAction(copy);
    wordsearchMenu->addAction(alphabetAct);
    QMenu *wordSpaceMenu = wordsearchMenu->addMenu(tr("Word Spaces"));
    wordSpaceMenu->addAction(wordSpaceFSAct);
    wordSpaceMenu->addAction(wordSpaceRSAct);
    wordSpaceMenu->addAction(wordSpaceKSAct);
    wordlistMenu->addAction(listAct);    
    QMenu *listorderMenu = wordlistMenu->addMenu(tr("List Order"));
    listorderMenu->addAction(listorderAOAct);
    listorderMenu->addAction(listorderAAAct);
    listorderMenu->addAction(listorderADAct);
    listorderMenu->addAction(listorderLDAct);
    listorderMenu->addAction(listorderLAAct);
    helpMenu->addAction(docAct);
    helpMenu->addAction(checkAct);
    helpMenu->addAction(aboutAct);
    QToolBar *toolbar = new QToolBar("File toolbar");
    //toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolbar->addAction(newAct);
    toolbar->addAction(openAct);
    toolbar->addAction(saveAct);
    toolbar->addAction(PDFAct);
    toolbar->addAction(DPrintAct);
//    toolbar->addAction(quitAct);
    addToolBar(toolbar);
    QToolBar *toolbar1 = new QToolBar("Play toolbar");
    //toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //toolbar1->addAction(allowplay);
    toolbar1->addAction(clearPlayAnswersAction);
    addToolBar(toolbar1);
#ifndef Q_OS_MAC
    QApplication::setWindowIcon(QIcon(":/icons/512x512icon.png"));
#endif
    WordSearchApplication *app = static_cast<WordSearchApplication*>(QApplication::instance());
#ifdef Q_OS_MAC
    //When the menu list is updated update the window menu
    connect(app, SIGNAL(windowListChanged()), this, SLOT(updateWindowMenu()));
    connect(app, SIGNAL(currentWindowChanged()), this, SLOT(setWindowCheck()));
#endif
    //Register the window
    app->registerWindow(this);

#ifdef Q_OS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
#endif
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::WindowActivate || event->type() == QEvent::WindowDeactivate)
    {;
        WordSearchApplication *app = static_cast<WordSearchApplication*>(QApplication::instance());
        app->setWindowCheck();
        if (event->type() == QEvent::WindowDeactivate)
        {
            foreach (QAction *action, actionsDisableOnMinimise)
            {
                action->setDisabled(true);
            }
        }
        else
        {
            foreach (QAction *action, actionsDisableOnMinimise)
            {
                action->setDisabled(false);
            }
            //This keeps getting hidden on minise, so show it.
            wscdock->show();
        }
    }
    return QMainWindow::event(event);
}

void MainWindow::ShowDoc()
{
    //LaunchBrowserDialog::launchBrowser(QUrl("http://openforschools.co.uk/wordsearchcreator/doc/"));
    QDesktopServices::openUrl(QUrl(QString("http://wordsearchcreator.org/doc/").append(UpdateChecker::getVersion())));
}

void MainWindow::CheckUpdate()
{
    WordSearchApplication *app = static_cast<WordSearchApplication*>(QApplication::instance());
    app->updateChecker->checkForUpdate(true);
}

void MainWindow::download()
{
    QDesktopServices::openUrl(QUrl("http://wordsearchcreator.org/wordsearches"));
}

void MainWindow::ShowAbout()
{
    QDialog *aboutDialog = new QDialog;
    QLabel *label = new QLabel(aboutDialog);
    label->setObjectName(QString::fromUtf8("label"));
    label->setPixmap(QPixmap(QString::fromUtf8(":/images/about.png")));
    label->setGeometry(QRect(QPoint(0, 0), label->pixmap()->size()));
    aboutDialog->setMinimumSize(QSize(label->pixmap()->size()));
    aboutDialog->setMaximumSize(QSize(label->pixmap()->size()));
    aboutDialog->setModal(true);
    aboutDialog->exec();
}

void MainWindow::changelistorder(QAction *listorderaction)
{
    wsdoc->wordlistorder=(WordSearchDoc::ordertype)listorderaction->data().toInt();
    wsdoc->setEditedState(true);
}

void MainWindow::changeWordSpace(QAction *action)
{
    wsdoc->setWordSpace((wordsearch::wordSpaceType)action->data().toInt());
    wsdoc->setEditedState(true);
}

void MainWindow::updateWindowMenu()
{
    WordSearchApplication *app = static_cast<WordSearchApplication*>(QApplication::instance());
    foreach (QAction *action, windowActions)
    {
        windowMenu->removeAction(action);
        delete action;
    }
    windowActions.clear();
    int i=0;
    foreach (MainWindow *window, app->windows)
    {
        QString title = window->windowTitle();
        //Remove the edited placeholder. Shoud be done better than this.
        title.remove("[*]");
        QAction *windowAction = new QAction(title, this);
        windowMenu->addAction(windowAction);
        windowAction->setCheckable(true);
#ifndef Q_OS_MAC
        windowAction->setShortcut(QString("Ctrl+").append(QString::number(i+1)));
#endif
        windowAction->setData(i);
        windowAction->setActionGroup(windowselectorGroup);
        if (window==this)
        {
            windowAction->setChecked(true);
            thisWindowAction=windowAction;
        }
        windowActions.append(windowAction);
        i++;
    }
}

void MainWindow::setWindowCheck()
{
    WordSearchApplication *app = static_cast<WordSearchApplication*>(QApplication::instance());
    int i=0;
    foreach (MainWindow *window, app->windows)
    {
        windowActions[i]->setChecked(window->isActiveWindow());
        i++;
    }
}

void MainWindow::launchPreferences()
{
    PreferencesDialog *pd = new PreferencesDialog();
    pd->exec();
}


void MainWindow::windowSelected(QAction *action)
{
    //One of the windows was selected off the menu
    WordSearchApplication *app = static_cast<WordSearchApplication*>(QApplication::instance());
    QWidget *selctedWindow = app->windows.at(action->data().toInt());
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
        selctedWindow->raise();
        selctedWindow->activateWindow();
    }
}

void MainWindow::maximise()
{
    setWindowState(windowState() ^ Qt::WindowMaximized);
}

void MainWindow::minimise()
{
    setWindowState(windowState()|Qt::WindowMinimized);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    WordSearchApplication *app = static_cast<WordSearchApplication*>(QApplication::instance());
    if (wsdoc->getEdited())
    {
		QMessageBox msgBox(this);
        msgBox.setWindowTitle(QApplication::applicationName());
        msgBox.setText(tr("The document has been modified.\n"
									 "Do you want to save your changes?"));
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		msgBox.setWindowModality(Qt::WindowModal);
        int ret = msgBox.exec();
        if ((ret == QMessageBox::Save && wsc->Save()) || ret == QMessageBox::Discard)
        {
            event->accept();
            app->deRegisterWindow(this);

        }
        else
            event->ignore();
    }
    else
    {
        event->accept();
        app->deRegisterWindow(this);
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    wsdraw->autoZoom();
}

MainWindow::~MainWindow()
{
    delete wsdoc;
}
