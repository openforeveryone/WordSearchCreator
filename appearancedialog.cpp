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

#include <QFontDialog>
#include <QColorDialog>

#include "wordsearchdrawer.h"

#include "appearancedialog.h"

appearanceDialog::appearanceDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    connect(titlefontButton, SIGNAL(clicked()), this, SLOT(titleFontPressed()));
    connect(wsfontButton, SIGNAL(clicked()), this, SLOT(wsFontPressed()));
    connect(wlfontButton, SIGNAL(clicked()), this, SLOT(wlFontPressed()));
    connect(titlecolorButton, SIGNAL(clicked()), this, SLOT(titleColorPressed()));
    connect(wscolorButton, SIGNAL(clicked()), this, SLOT(wsColorPressed()));
    connect(wlcolorButton, SIGNAL(clicked()), this, SLOT(wlColorPressed()));
    connect(bgcolorButton, SIGNAL(clicked()), this, SLOT(bgColorPressed()));
    connect(hlcolorButton, SIGNAL(clicked()), this, SLOT(hlColorPressed()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(gridColorButton, SIGNAL(clicked()), this, SLOT(gridColorPressed()));
    connect(gridlinewidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(gridWidthChange(double)));
}

void appearanceDialog::setDoc(WordSearchDoc *wsd)
{
    this->wsd = wsd;
    titleFont=(wsd->getTitleFont());
    titlefontLabel->setFont(titleFont);
    titlefontLabel->setText(titleFont.family());
    wsFont=(wsd->getWSFont());
    wsfontLabel->setFont(wsFont);
    wsfontLabel->setText(wsFont.family());
    wlFont=(wsd->getWordsFont());
    wlfontLabel->setFont(wlFont);
    wlfontLabel->setText(wlFont.family());
    titleColor=(wsd->getTitleColor());
    wsColor=(wsd->getWSColor());
    wlColor=(wsd->getWLColor());
    bgColor=(wsd->getBGColor());
    hlColor=(wsd->getHLColor());
    gridColor=(wsd->getGridColor());
    QPalette palette;
    palette.setColor(QPalette::Active, QPalette::WindowText, wsColor);
    wsfontLabel->setPalette(palette);
    palette.setColor(QPalette::Active, QPalette::WindowText, titleColor);
    titlefontLabel->setPalette(palette);
    palette.setColor(QPalette::Active, QPalette::WindowText, wlColor);
    wlfontLabel->setPalette(palette);
    palette.setColor(QPalette::Active, QPalette::WindowText, gridColor);
    //gridlineframe->setPalette(palette);
    gridWidth=wsd->getGridWidth();
    gridlinewidthSpinBox->setValue(gridWidth);
    showGridCheckBox->setChecked(wsd->getShowGrid());
}

//QString appearanceDialog::fontstring(QFont font)
//{
//    return font.family()+QString(font.pointSize());
//}

void appearanceDialog::titleFontPressed()
{
    bool ok;
    titleFont = QFontDialog::getFont(&ok, titlefontLabel->font(), this, "Title Font", QFontDialog::DontUseNativeDialog);
    titlefontLabel->setFont(titleFont);
    titlefontLabel->setText(titlefontLabel->font().family());
}

void appearanceDialog::titleColorPressed()
{
    QPalette palette;
    QColor color;
    color=(QColorDialog::getColor(titleColor, this));
    if (color.isValid())
        titleColor=color;
    palette.setColor(QPalette::Active, QPalette::WindowText, titleColor);
    titlefontLabel->setPalette(palette);
}

void appearanceDialog::wsFontPressed()
{
    bool ok;
    wsFont = QFontDialog::getFont(&ok, wsfontLabel->font(), this, "Word Search Font", QFontDialog::DontUseNativeDialog);
    wsfontLabel->setFont(wsFont);
    wsfontLabel->setText(wsfontLabel->font().family());
}

void appearanceDialog::wsColorPressed()
{
    QPalette palette;
    QColor color;
    color=(QColorDialog::getColor(wsColor, this));
    if (color.isValid())
        wsColor=color;
    palette.setColor(QPalette::Active, QPalette::WindowText, wsColor);
    wsfontLabel->setPalette(palette);
}

void appearanceDialog::wlFontPressed()
{
    bool ok;
    wlFont = QFontDialog::getFont(&ok, wlfontLabel->font(), this, "Word List Font", QFontDialog::DontUseNativeDialog);
    wlfontLabel->setFont(wlFont);
    wlfontLabel->setText(wlfontLabel->font().family());
}

void appearanceDialog::wlColorPressed()
{
    QPalette palette;
    QColor color;
    color=(QColorDialog::getColor(wlColor, this));
    if (color.isValid())
        wlColor=color;
    palette.setColor(QPalette::Active, QPalette::WindowText, wlColor);
    wlfontLabel->setPalette(palette);
}

void appearanceDialog::bgColorPressed()
{
    QColor color;
    color=(QColorDialog::getColor(bgColor, this));
    if (color.isValid())
        bgColor=color;
}

void appearanceDialog::hlColorPressed()
{
    QColor color;
    color=(QColorDialog::getColor(hlColor, this));
    if (color.isValid())
        hlColor=color;
}

void appearanceDialog::gridColorPressed()
{
    QPalette palette;
    QColor color;
    color=(QColorDialog::getColor(gridColor, this));
    if (color.isValid())
        gridColor=color;
    palette.setColor(QPalette::Active, QPalette::WindowText, gridColor);
    //gridlineframe->setPalette(palette);
}

void appearanceDialog::gridWidthChange(double width)
{
    gridWidth = width;
    //gridlineframe->setLineWidth(width);
}

void appearanceDialog::okPressed()
{
    wsd->setTitleFont(titleFont);
    wsd->setWSFont(wsFont);
    wsd->setWordsFont(wlFont);
    wsd->setTitleColor(titleColor);
    wsd->setWSColor(wsColor);
    wsd->setWordsColor(wlColor);
    wsd->setBgColor(bgColor);
    wsd->setHlColor(hlColor);
    wsd->setGridColor(gridColor);
    wsd->setGridWidth(gridWidth);
    wsd->setShowGrid(showGridCheckBox->isChecked());
    accept();
}
