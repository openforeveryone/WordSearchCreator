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

#ifndef DIRECTIONSELECTORDIALOGUE_H
#define DIRECTIONSELECTORDIALOGUE_H

#include <QDialog>

class DirectionSelector;
class wordsearch;

class DirectionSelectorDialog: public QDialog
{
	Q_OBJECT

	public:
		DirectionSelectorDialog(wordsearch *wordsearch, QWidget *parent = 0);

	private:
		DirectionSelector *ds;

	signals:
		void angleChanged(double mouseAngle);
};

#endif
