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

#include <QPainter>
#include <QPaintEvent>
#include <QtPrintSupport/QPrintDialog>

#include "pagelayoutdialogue.h"

PageLayoutDialog::PageLayoutDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    PageLayoutView *plv = new PageLayoutView(this);
    gridLayout->addWidget(plv, 0, 1, 1, 1);
    populatePaperSizes(pagesizecombo);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(heightSpinBox, SIGNAL(valueChanged(double)), plv, SLOT(setPageHeight(double)));
    connect(widthSpinBox, SIGNAL(valueChanged(double)), plv, SLOT(setPageWidth(double)));
    connect(leftSpinBox, SIGNAL(valueChanged(double)), plv, SLOT(setMarginLeft(double)));
    connect(topSpinBox, SIGNAL(valueChanged(double)), plv, SLOT(setMarginTop(double)));
    connect(rightSpinBox, SIGNAL(valueChanged(double)), plv, SLOT(setMarginRight(double)));
    connect(bottomSpinBox, SIGNAL(valueChanged(double)), plv, SLOT(setMarginBottom(double)));
    connect(heightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PageSizeChanged()));
    connect(widthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(PageSizeChanged()));
    connect(leftSpinBox, SIGNAL(valueChanged(double)), this, SLOT(MarginChanged()));
    connect(topSpinBox, SIGNAL(valueChanged(double)), this, SLOT(MarginChanged()));
    connect(rightSpinBox, SIGNAL(valueChanged(double)), this, SLOT(MarginChanged()));
    connect(bottomSpinBox, SIGNAL(valueChanged(double)), this, SLOT(MarginChanged()));
    connect(portraitRB, SIGNAL(clicked(bool)), this, SLOT(setPortrait()));
    connect(landscapeRB, SIGNAL(clicked(bool)), this, SLOT(setLandscape()));
    connect(pagesizecombo, SIGNAL(currentIndexChanged(int)), this, SLOT(PageSizeComboChanged(int)));
    //widthSpinBox->setValue(20);
    //heightSpinBox->setValue(10);
    pagesizecombo->setCurrentIndex(pagesizecombo->findData(QPageSize::A4));
    topSpinBox->setValue(1);
    rightSpinBox->setValue(1);
    bottomSpinBox->setValue(1);
    leftSpinBox->setValue(1);
    PageSizeChanged();
    //connect(this, SIGNAL(accepted()), ds, SLOT(commit()));
}

void PageLayoutDialog::PageSizeChanged()
{
    if (heightSpinBox->value() >= widthSpinBox->value())
        portraitRB->setChecked(true);
    else
        landscapeRB->setChecked(true);
    topSpinBox->setMaximum(heightSpinBox->value()-(bottomSpinBox->value()+0.1));
    rightSpinBox->setMaximum(widthSpinBox->value()-(leftSpinBox->value()+0.1));
    bottomSpinBox->setMaximum(heightSpinBox->value()-(topSpinBox->value()+0.1));
    leftSpinBox->setMaximum(widthSpinBox->value()-(rightSpinBox->value()+0.1));
    int index = pagesizecombo->findData(QPageSize::Custom);
    for (int a = 0; a < QPageSize::Custom; a++)
    {
        if(((PageLayoutDialog::paperSize(static_cast<QPageSize::PageSizeId>(a)).width==qRound(widthSpinBox->value()*72.0))&&(PageLayoutDialog::paperSize(static_cast<QPageSize::PageSizeId>(a)).height==qRound(heightSpinBox->value()*72.0)))||((PageLayoutDialog::paperSize(static_cast<QPageSize::PageSizeId>(a)).width==qRound(heightSpinBox->value()*72.0))&&(PageLayoutDialog::paperSize(static_cast<QPageSize::PageSizeId>(a)).height==qRound(widthSpinBox->value()*72.0))))
        {
            index=(pagesizecombo->findData(static_cast<QPageSize::PageSizeId>(a)));
        }
    }
    pagesizecombo->setCurrentIndex(index);
}

void PageLayoutDialog::MarginChanged()
{

}

void PageLayoutDialog::PageSizeComboChanged(int index)
{
    PageLayoutDialog::PaperSize ps;
    QPageSize::PageSizeId qpps = static_cast<QPageSize::PageSizeId>(pagesizecombo->itemData(index).toInt());
    if (qpps == QPageSize::PageSizeId::Custom)
        return;
    ps = PageLayoutDialog::paperSize(qpps);
    if (portraitRB->isChecked()==true)
    {
        heightSpinBox->setValue(ps.height/72.0);
        widthSpinBox->setValue(ps.width/72.0);
    }
    else
    {
        heightSpinBox->setValue(ps.width/72.0);
        widthSpinBox->setValue(ps.height/72.0);
    }
}

void PageLayoutDialog::setPortrait()
{
    if (heightSpinBox->value() < widthSpinBox->value())
    {
        double width = widthSpinBox->value();
        widthSpinBox->setValue(heightSpinBox->value());
        heightSpinBox->setValue(width);
    }
}

void PageLayoutDialog::setLandscape()
{
    if (heightSpinBox->value() > widthSpinBox->value())
    {
        double width = widthSpinBox->value();
        widthSpinBox->setValue(heightSpinBox->value());
        heightSpinBox->setValue(width);
    }
}


PageLayoutView::PageLayoutView(QWidget *parent)
    : QWidget(parent)
{
    setPageWidth(10);
    setPageHeight(15);
    setMarginTop(2);
    setMarginLeft(2);
    setMarginRight(2);
    setMarginBottom(2);
}

void PageLayoutView::paintEvent(QPaintEvent *event)
{
    event->accept();
    QPainter painter(this);
    QBrush brush(Qt::white);
    QPen pagePen(Qt::black);
    pagePen.setWidth(0);
    QPen marginPen(Qt::gray);
    marginPen.setWidth(0);

    double wr = width()/(PageWidth+1);
    double hr = height()/(PageHeight+1);
    painter.scale(qMin(wr, hr),qMin(wr, hr));
    painter.translate(((width()/qMin(wr, hr)-(PageWidth))/2),((height()/qMin(wr, hr)-(PageHeight))/2));
    painter.fillRect(QRectF(0,0,PageWidth,PageHeight),brush);
    painter.setPen(pagePen);
    painter.drawRect(QRectF(0,0,PageWidth,PageHeight));
    painter.setPen(marginPen);
    painter.drawRect(QRectF(MarginLeft, MarginTop, PageWidth - MarginLeft - MarginRight, PageHeight - MarginTop - MarginBottom));
}

void PageLayoutView::setPageWidth(double Width)
{
    PageWidth=Width;
    update();
}

void PageLayoutView::setPageHeight(double Height)
{
    PageHeight=Height;
    update();
}

void PageLayoutView::setMarginTop(double Height)
{
    MarginTop=Height;
    update();
}

void PageLayoutView::setMarginLeft(double Width)
{
    MarginLeft=Width;
    update();
}

void PageLayoutView::setMarginRight(double Width)
{
    MarginRight=Width;
    update();
}

void PageLayoutView::setMarginBottom(double Height)
{
    MarginBottom=Height;
    update();
}

#define Q_MM(n) int((n * 720 + 127) / 254)
#define Q_IN(n) int(n * 72)

static const PageLayoutDialog::PaperSize paperSizes[QPageSize::PageSizeId::Custom] =
{
    {  Q_MM(210), Q_MM(297) },      // A4
    {  Q_MM(176), Q_MM(250) },      // B5
    {  Q_IN(8.5), Q_IN(11) },       // Letter
    {  Q_IN(8.5), Q_IN(14) },       // Legal
    {  Q_IN(7.5), Q_IN(10) },       // Executive
    {  Q_MM(841), Q_MM(1189) },     // A0
    {  Q_MM(594), Q_MM(841) },      // A1
    {  Q_MM(420), Q_MM(594) },      // A2
    {  Q_MM(297), Q_MM(420) },      // A3
    {  Q_MM(148), Q_MM(210) },      // A5
    {  Q_MM(105), Q_MM(148) },      // A6
    {  Q_MM(74), Q_MM(105)},        // A7
    {  Q_MM(52), Q_MM(74) },        // A8
    {  Q_MM(37), Q_MM(52) },        // A9
    {  Q_MM(1000), Q_MM(1414) },    // B0
    {  Q_MM(707), Q_MM(1000) },     // B1
    {  Q_MM(31), Q_MM(44) },        // B10
    {  Q_MM(500), Q_MM(707) },      // B2
    {  Q_MM(353), Q_MM(500) },      // B3
    {  Q_MM(250), Q_MM(353) },      // B4
    {  Q_MM(125), Q_MM(176) },      // B6
    {  Q_MM(88), Q_MM(125) },       // B7
    {  Q_MM(62), Q_MM(88) },        // B8
    {  Q_MM(44), Q_MM(62) },        // B9
    {  Q_MM(162),    Q_MM(229) },   // C5E
    {  Q_IN(4.125),  Q_IN(9.5) },   // Comm10E
    {  Q_MM(110),    Q_MM(220) },   // DLE
    {  Q_IN(8.5),    Q_IN(13) },    // Folio
    {  Q_IN(17),     Q_IN(11) },    // Ledger
    {  Q_IN(11),     Q_IN(17) }     // Tabloid
};

void PageLayoutDialog::populatePaperSizes(QComboBox* cb)
{
    cb->addItem(QPrintDialog::tr("A0 (841 x 1189 mm)"), QPageSize::A0);
    cb->addItem(QPrintDialog::tr("A1 (594 x 841 mm)"), QPageSize::A1);
    cb->addItem(QPrintDialog::tr("A2 (420 x 594 mm)"), QPageSize::A2);
    cb->addItem(QPrintDialog::tr("A3 (297 x 420 mm)"), QPageSize::A3);
    cb->addItem(QPrintDialog::tr("A4 (210 x 297 mm)"), QPageSize::A4);
    cb->addItem(QPrintDialog::tr("A5 (148 x 210 mm)"), QPageSize::A5);
    cb->addItem(QPrintDialog::tr("A6 (105 x 148 mm)"), QPageSize::A6);
    cb->addItem(QPrintDialog::tr("A7 (74 x 105 mm)"), QPageSize::A7);
    cb->addItem(QPrintDialog::tr("A8 (52 x 74 mm)"), QPageSize::A8);
    cb->addItem(QPrintDialog::tr("A9 (37 x 52 mm)"), QPageSize::A9);
    cb->addItem(QPrintDialog::tr("B0 (1000 x 1414 mm)"), QPageSize::B0);
    cb->addItem(QPrintDialog::tr("B1 (707 x 1000 mm)"), QPageSize::B1);
    cb->addItem(QPrintDialog::tr("B2 (500 x 707 mm)"), QPageSize::B2);
    cb->addItem(QPrintDialog::tr("B3 (353 x 500 mm)"), QPageSize::B3);
    cb->addItem(QPrintDialog::tr("B4 (250 x 353 mm)"), QPageSize::B4);
    cb->addItem(QPrintDialog::tr("B5 (176 x 250 mm)"), QPageSize::B5);
    cb->addItem(QPrintDialog::tr("B6 (125 x 176 mm)"), QPageSize::B6);
    cb->addItem(QPrintDialog::tr("B7 (88 x 125 mm)"), QPageSize::B7);
    cb->addItem(QPrintDialog::tr("B8 (62 x 88 mm)"), QPageSize::B8);
    cb->addItem(QPrintDialog::tr("B9 (44 x 62 mm)"), QPageSize::B9);
    cb->addItem(QPrintDialog::tr("B10 (31 x 44 mm)"), QPageSize::B10);
    cb->addItem(QPrintDialog::tr("C5E (163 x 229 mm)"), QPageSize::C5E);
    cb->addItem(QPrintDialog::tr("DLE (110 x 220 mm)"), QPageSize::DLE);
    cb->addItem(QPrintDialog::tr("Executive (191 x 254 mm)"), QPageSize::Executive);
    cb->addItem(QPrintDialog::tr("Folio (210 x 330 mm)"), QPageSize::Folio);
    cb->addItem(QPrintDialog::tr("Ledger (432 x 279 mm)"), QPageSize::Ledger);
    cb->addItem(QPrintDialog::tr("Legal (216 x 356 mm)"), QPageSize::Legal);
    cb->addItem(QPrintDialog::tr("Letter (216 x 279 mm)"), QPageSize::Letter);
    cb->addItem(QPrintDialog::tr("Tabloid (279 x 432 mm)"), QPageSize::Tabloid);
    cb->addItem(QPrintDialog::tr("Custom"), QPageSize::Custom);
}

PageLayoutDialog::PaperSize PageLayoutDialog::paperSize(QPageSize pageSize)
{
    return paperSizes[pageSize.id()];
}
