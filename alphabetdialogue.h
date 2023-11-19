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

#ifndef APHABETDIALOGUE_H
#define APHABETDIALOGUE_H

#include "ui_alphabet.h"

class WordSearchDoc;
class QNetworkAccessManager;
class QNetworkReply;
class QProgressDialog;

class AlphabetDialog : public QDialog, private Ui::alphabet
{
    Q_OBJECT

public:
    AlphabetDialog(QWidget *parent = 0);
    void setDoc(WordSearchDoc *wsd);

private:
    WordSearchDoc *wsd;
    QNetworkAccessManager *networkAccessManager;
    QProgressDialog *progressDialog;

public slots:
    void okPressed();
    void cboxchanged(int index);
    void toUpper();
    void toLower();
    void abedeted(QString text);
    void save();
    void getfromweb();
    void httpRequestFinished(QNetworkReply *reply);
};

#endif
