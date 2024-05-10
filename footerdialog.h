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

#ifndef FOOTERDIALOG_H
#define FOOTERDIALOG_H

#include "ui_footer.h"

class WordSearchDoc;

class footerdialog : public QDialog, private Ui::footer
{
    Q_OBJECT

public:
    explicit footerdialog(QWidget *parent = nullptr);
    void setDoc(WordSearchDoc *wsd);

private slots:
    void okPressed();

    void on_customFooterCheckBox_toggled(bool checked);

private:
    WordSearchDoc *wsd;

};

#endif // FOOTERDIALOG_H
