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

#ifndef DIRECTIONSELECTOR_H
#define DIRECTIONSELECTOR_H

#include <QWidget>

class wordsearch;

class DirectionSelector : public QWidget
{
	Q_OBJECT

	public:
		DirectionSelector(QWidget *parent = 0);
		void setWS(wordsearch *wordsearch);

	public slots:
		void commit();

	protected:
		void paintEvent(QPaintEvent *event);
		void mouseMoveEvent( QMouseEvent * e );
		void mousePressEvent( QMouseEvent * e );

	private:
		wordsearch *ws;
		int boxsize;
		QRegion directionRegion[8];
		bool direction[8];
		bool firstdirection;
		int hldirection;
		double mouseAngle(QPoint mousePos);
		void drawArrow(QPainter *painter);

	signals:
		void angleChanged(double mouseAngle);
};

#endif
