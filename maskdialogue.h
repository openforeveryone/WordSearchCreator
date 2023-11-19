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

#ifndef MASKDIALOGUE_H
#define MASKDIALOGUE_H

#include <QDialog>

class QLabel;
class QCheckBox;

class WordSearchDoc;
class MaskDrawer;

class MaskDialog : public QDialog
{
    Q_OBJECT

public:
    MaskDialog(WordSearchDoc *wsd =0, QWidget *parent = 0);

private:
    QLabel * label;
    void ChangeLabel();
    WordSearchDoc *wsd;
    QCheckBox *usemaskcheckbox;
    MaskDrawer *md;

public slots:
    void setUseMask(bool useMask);
};

#endif
