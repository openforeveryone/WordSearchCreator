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

#include <QSettings>

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include "wordsearchapplication.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    QSettings settings;
    bool autoCheckEnabled = settings.value("updateChecker/autoUpdateCheckEnabled", true).toBool();
    ui->updatesCheckBox->setChecked(autoCheckEnabled);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::on_buttonBox_accepted()
{
    QSettings settings;
    settings.setValue("updateChecker/autoUpdateCheckEnabled",  ui->updatesCheckBox->checkState());
}
