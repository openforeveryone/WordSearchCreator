/***************************************************************************
 *   Copyright (C) 2006-2013 M Wellings                                    *
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

#ifndef MASKDRAWER_H
#define MASKDRAWER_H

#include <QWidget>

class WordSearchDoc;
class wordsearch;

class MaskDrawer : public QWidget
{
    Q_OBJECT

public:
    MaskDrawer(WordSearchDoc *wsd, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QSize sizeHint();

private:
    wordsearch *ws;
    int boxsize;
    WordSearchDoc *wsd;
};

#endif
