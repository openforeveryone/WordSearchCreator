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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "directionselectordialogue.h"

#include "directionselector.h"

DirectionSelectorDialog::DirectionSelectorDialog(wordsearch *wordsearch, QWidget *parent)
    : QDialog(parent)
{
    this->setWindowModality(Qt::WindowModal);
    this->setWindowTitle("Set word directions");
    ds = new DirectionSelector(this);
    ds->setWS(wordsearch);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ds);
    QHBoxLayout *layout1 = new QHBoxLayout;
    QPushButton *okButton = new QPushButton("&OK", this);
    QPushButton *cancelButton = new QPushButton("&Cancel", this);
    layout1->addStretch();
    layout1->addWidget(okButton);
    layout1->addWidget(cancelButton);
    layout->addLayout(layout1);
    this->setLayout(layout);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), ds, SLOT(commit()));
}
