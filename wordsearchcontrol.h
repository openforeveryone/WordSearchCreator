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

#ifndef WORDSEARCHCONTROL_H
#define WORDSEARCHCONTROL_H

#include <QBuffer>
#include <QDialog>

#include "ui_wordsearchcontrol.h"

class WordSearchDoc;

class WordSearchControl : public QWidget , private Ui::WordSearchControl
{
    Q_OBJECT

private:
    QTextEdit *wlb;
    WordSearchDoc *wsd;
    QBuffer *webbuffer;
    //		QHttp *http;
    int httpPostId;
private slots:
    void createclicked();
    void UpdateConts();
public:
    WordSearchControl(QWidget *parent = 0);
    void setWordSearchDoc (WordSearchDoc *wsd);
    void SetupTemplate();
public slots:
    void Open(QString fileName = "");
    int Save();
    int SaveAs();
    void New();
    void appearance();
    void showdirectionselector();
    void showpageLayout();
    void showmaskdrawer();
    void showlistCreator();
    void updateNoLines();
    void alphabet();
    //		void upload();
    //        void httpRequestFinished(int requestId, bool error);
    void useMaskChanged(int used);
    void setFooter();
signals:
    void Createclicked();
    void WSFontchanged(QFont WSFont);
    void Fontclicked();
    void Sizechanged(int x, int y);
    void arsstateChanged(int value);
    void sastateChanged(int value);
    void swstateChanged(int value);
    void titleChanged(QString Title);
    void XValChanged(int value);
    void YValChanged(int value);
    void umstateChanged(int value);
};

class createDialog : public QDialog
{
    Q_OBJECT

public:
    createDialog(QWidget *parent = 0, WordSearchDoc *wsd = 0);
private:
    WordSearchDoc *wsd;
private slots:
    void createws();
    void stop();
};

#endif
