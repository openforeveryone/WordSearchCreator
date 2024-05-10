/***************************************************************************
 *   Copyright (C) 2006-2024 M Wellings                                    *
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

#include <QDebug>

#include "wordsearchdoc.h"

#include "footerdialog.h"

footerdialog::footerdialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
}

void footerdialog::setDoc(WordSearchDoc *wsd)
{
    this->wsd = wsd;
    customFooterCheckBox->setChecked(!wsd->getFooter().isNull());
    if (!wsd->getFooter().isNull())
        footerTextLineEdit->setText(wsd->getFooter());
    else
        footerTextLineEdit->setText(WordSearchDoc::getDefaultFooter());
    if (wsd->getShowPageNumber() == WordSearchDoc::showPageNumberType::PageNumberNever)
        noPageNumberRadioButton->setChecked(true);
    else if (wsd->getShowPageNumber() == WordSearchDoc::showPageNumberType::PageNumberAlways)
        alwaysPageNumberRadioButton->setChecked(true);
    noPageCountRadioButton->setChecked(!wsd->getShowPageCount());
}

void footerdialog::okPressed()
{
    if (customFooterCheckBox->isChecked())
        wsd->setFooter(footerTextLineEdit->text());
    else
        wsd->setFooter(QString());
    if (noPageNumberRadioButton->isChecked())
        wsd->setShowPageNumber(WordSearchDoc::showPageNumberType::PageNumberNever);
    else if (multiplePageNumberRadioButton->isChecked())
        wsd->setShowPageNumber(WordSearchDoc::showPageNumberType::PageNumberMultipage);
    else
        wsd->setShowPageNumber(WordSearchDoc::showPageNumberType::PageNumberAlways);
    wsd->setShowPageCount(pageCountRadioButton->isChecked());
    accept();
}

void footerdialog::on_customFooterCheckBox_toggled(bool checked)
{
    qDebug() << "on_cudtomFooterCheckBox_toggled";
    footerLabel->setEnabled(checked);
    footerTextLineEdit->setEnabled(checked);
    if (!checked)
        footerTextLineEdit->setText(WordSearchDoc::getDefaultFooter());
    else
        footerTextLineEdit->setText("");
}
