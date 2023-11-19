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

#include <QtGui>

#include "directionselector.h"

#include "wordsearch.h"

DirectionSelector::DirectionSelector(QWidget *parent)
	: QWidget(parent)
{
	this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	for (int x = 0; x < 8; x++)
		direction[x]=false;
	QPalette palette(QColor(255, 255, 255));
	this->setPalette(palette);
	this->setAutoFillBackground(true);
	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMinimumSize(180,180);
}

void DirectionSelector::setWS(wordsearch *wordsearch)
{
	ws = wordsearch;
	for (int x = 0; x < 8; x++)
		direction[x]=ws->DirectionAllowed(x);
}

void DirectionSelector::commit()
{
	for (int x = 0; x < 8; x++)
		ws->setDirectionAllowed(x,direction[x]);
}

void DirectionSelector::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawRect(0,0,width()-1,height()-1);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(width()/2, height()/2);
	int side = qMin(width(), height());
	painter.scale(side / 150.0, side / 150.0);
	QPen pen1(Qt::black);
	QPen pen2(Qt::gray);
	painter.setPen(pen1);
	for (int x = 0; x < 8; x++)
	{
		if (direction[x])
			painter.setPen(pen1);
		else
			painter.setPen(pen2);
		drawArrow(&painter);
		painter.rotate(45);
	}
}

double DirectionSelector::mouseAngle(QPoint mousePos)
{
	int x = mousePos.x(), y = mousePos.y();
	y = y * -1;
	if ((y == 0) && (x >= 0))
		return 0;
	if ((y == 0) && (x < 0))
		return M_PI;
	if ((x == 0) && (y > 0))
		return M_PI_2;
	if ((x == 0) && (y < 0))
		return M_PI+M_PI_2;;
	double angle = atan(double(y)/double(x));
	if (x < 0)
		angle = M_PI - -1.0 * angle;
	if ((x > 0) && (y < 0))
		angle = 2 * M_PI + angle;
	return angle;
}

void DirectionSelector::mouseMoveEvent ( QMouseEvent * e )
{
	QPoint mousePos(e->pos());
	mousePos.setX(mousePos.x()-(width()/2));
	mousePos.setY(mousePos.y()-(height()/2));
	double mouseAngle = this->mouseAngle(mousePos);
	mouseAngle = 2 * M_PI - mouseAngle;
	mouseAngle = mouseAngle - (M_PI+M_PI_2-(M_PI/8));
	if (mouseAngle < 0) mouseAngle = mouseAngle + 2 * M_PI;
	int tdirection = int(mouseAngle / (M_PI/4));
	if (hldirection!=tdirection)
	{
		hldirection = tdirection;
		update();
	}
	if (e->buttons() & Qt::LeftButton)
		direction[tdirection] = firstdirection;
}

void DirectionSelector::drawArrow(QPainter *painter)
{
	painter->drawLine(7,-17,7,-50);
	painter->drawLine(-7,-17,-7,-50);
	painter->drawLine(15,-50,7,-50);
	painter->drawLine(-15,-50,-7,-50);
	painter->drawLine(15,-50,0,-65);
	painter->drawLine(-15,-50,0,-65);
}

void DirectionSelector::mousePressEvent(QMouseEvent *e)
{
	QPoint mousePos(e->pos());
	mousePos.setX(mousePos.x()-(width()/2));
	mousePos.setY(mousePos.y()-(height()/2));
	double mouseAngle = this->mouseAngle(mousePos);
	mouseAngle = 2 * M_PI - mouseAngle;
	mouseAngle = mouseAngle - (M_PI+M_PI_2-(M_PI/8));
	if (mouseAngle < 0) mouseAngle = mouseAngle + 2 * M_PI;
	int tdirection = int(mouseAngle / (M_PI/4));
	if (direction[tdirection] == false)
		firstdirection = true;
	else
		firstdirection = false;
	direction[tdirection] = firstdirection;
	update();
}

