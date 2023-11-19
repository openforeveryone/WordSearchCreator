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

#include <QPainter>
#include <QMouseEvent>

#include "wordsearchdoc.h"

#include "maskdrawer.h"

MaskDrawer::MaskDrawer(WordSearchDoc *wsd, QWidget *parent)
    : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    ws = wsd->ws;
    this->wsd = wsd;
    if (!wsd->getUseMask())
        this->setEnabled(false);
    boxsize = 10;
    setMinimumSize ((ws->XSize()*boxsize)+1,(ws->YSize()*boxsize)+1);
    //setMinimumSize(180,180);
}

void MaskDrawer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QBrush brush1(Qt::white);
    QBrush brush2(Qt::black);
    painter.setBrush(brush1);
    for (int x = 0; x < ws->XSize(); x++)
    {
        for (int y = 0; y < ws->YSize(); y++)
        {
            if (ws->Mask(x+1,y+1)) {painter.setBrush(brush1);} else {painter.setBrush(brush2);}
            painter.drawRect ((x * boxsize), (y * boxsize), boxsize, boxsize);
        }
    }
}

void MaskDrawer::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        ws->allowLetter(true, event->x()/boxsize, event->y()/boxsize);
    }
    else if (event->buttons() == Qt::RightButton)
    {
        ws->allowLetter(false, event->x()/boxsize, event->y()/boxsize);
    }
    this->update();
    wsd->setMaskChanged(true);
}

void MaskDrawer::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        ws->allowLetter(true, event->x()/boxsize, event->y()/boxsize);
    }
    else if (event->buttons() == Qt::RightButton)
    {
        ws->allowLetter(false, event->x()/boxsize, event->y()/boxsize);
    }
    this->update();
    wsd->setMaskChanged(true);
}

QSize MaskDrawer::sizeHint()
{
    return QSize((ws->XSize()*boxsize)+1,(ws->YSize()*boxsize)+1);
}
