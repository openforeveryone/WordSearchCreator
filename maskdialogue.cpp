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
#include <QMessageBox>
#include <QLabel>
#include <QCheckBox>

#include "wordsearchdoc.h"
#include "maskdrawer.h"

#include "maskdialogue.h"

MaskDialog::MaskDialog(WordSearchDoc *wsd, QWidget *parent)
    : QDialog(parent)
{
    this->wsd=wsd;
    this->setWindowModality(Qt::WindowModal);
    label = new QLabel();
    ChangeLabel();
    label->setWordWrap(true);
    md = new MaskDrawer(wsd, this);
    usemaskcheckbox = new QCheckBox("Use shape mask", this);
    usemaskcheckbox->setChecked(wsd->getUseMask());
    connect(usemaskcheckbox, SIGNAL(toggled(bool)), this, SLOT(setUseMask(bool)));
    //connect(usemaskcheckbox, SIGNAL(stateChanged(int)), wsd, SLOT(setUseMask(int)));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(usemaskcheckbox);
    layout->addWidget(md);
    QHBoxLayout *layout1 = new QHBoxLayout;
    QPushButton *doneButton = new QPushButton("&Done", this);
    layout1->addStretch();
    layout1->addWidget(doneButton);
    layout->addLayout(layout1);
    this->setLayout(layout);
    this->setWindowTitle("Mask Drawer");
    connect(doneButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void MaskDialog::ChangeLabel()
{
    if (wsd->getUseMask())
        label->setText(tr("Click on a cell to enable it and right click to dissable it."));
    else
        label->setText(tr("Tick the use shape mask option below to enable shaped wordsearches."));
}

void MaskDialog::setUseMask(bool useMask)
{
    if (!useMask)
    {
        int cont;
        if (wsd->getMaskChanged())
            cont = QMessageBox::warning(this, tr("Save Wordsearch"), "This will delete the shape. Do you want to continue?", QMessageBox::Yes | QMessageBox::No);
        else
            cont = QMessageBox::Yes;
        if (cont == QMessageBox::Yes)
        {
            wsd->ws->resetmask();
            md->setEnabled(false);
            wsd->setUseMask(false);
            wsd->setMaskChanged(false);
        }
        else
        {
            usemaskcheckbox->setChecked(true);
        }
    }
    else
    {
        md->setEnabled(true);
        wsd->setUseMask(true);
    }
    ChangeLabel();
}

