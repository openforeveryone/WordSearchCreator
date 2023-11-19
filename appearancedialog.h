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

#ifndef APPEARANCE_H
#define APPEARANCE_H

#include "ui_appearance.h"

class WordSearchDoc;

class appearanceDialog : public QDialog, private Ui::appearance
{
    Q_OBJECT

public:
    appearanceDialog(QWidget *parent = 0);
    void setDoc(WordSearchDoc *wsd);

private:
    WordSearchDoc *wsd;
    QString fontstring(QFont font);
    QFont titleFont;
    QFont wsFont;
    QFont wlFont;
    QColor titleColor;
    QColor wsColor;
    QColor wlColor;
    QColor bgColor;
    QColor hlColor;
    QColor gridColor;
    double gridWidth;

public slots:
    void titleFontPressed();
    void wsFontPressed();
    void wlFontPressed();
    void titleColorPressed();
    void wsColorPressed();
    void wlColorPressed();
    void bgColorPressed();
    void hlColorPressed();
    void gridColorPressed();
    void okPressed();
    void gridWidthChange(double width);
};

#endif
