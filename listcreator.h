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

#ifndef LISTCREATOR_H
#define LISTCREATOR_H

#include "ui_listcreator.h"

class listCreator : public QDialog, private Ui::listcreator
{
	Q_OBJECT

	public:
		listCreator(QWidget *parent = 0);
		QStringList wl;
		bool load(const QString &f);

	public slots:
		void generatelist();
		void fileOpen();
		void okPressed();
		void help();		
};

#endif
