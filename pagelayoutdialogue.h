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

#ifndef PAGELAYOUTDIALOGUE_H
#define PAGELAYOUTDIALOGUE_H

#include <QtPrintSupport/QPrinter>

#include "ui_pagelayout.h"

class QPrinter;

class PageLayoutDialog: public QDialog, private Ui::LayoutDialog
{
    Q_OBJECT

public:
    PageLayoutDialog(QWidget *parent = 0);
    struct PaperSize {
        int width, height;
    };
    static PaperSize paperSize(QPrinter::PageSize pageSize);
    double PageWidth() {return widthSpinBox->value();}
    double PageHeight() {return heightSpinBox->value();}
    double MarginTop(){return topSpinBox->value();}
    double MarginLeft(){return leftSpinBox->value();}
    double MarginRight(){return rightSpinBox->value();}
    double MarginBottom(){return bottomSpinBox->value();}
    QPrinter::PageSize qpps() {return static_cast<QPrinter::PageSize>(pagesizecombo->itemData(pagesizecombo->currentIndex()).toInt());}

private:
    void populatePaperSizes(QComboBox* cb);
    //DirectionSelector *ds;

signals:
    //void angleChanged(double mouseAngle);

private slots:
    void PageSizeChanged();
    void setPortrait();
    void setLandscape();
    void MarginChanged();
    void PageSizeComboChanged(int index);

public slots:
    void setPageWidth(double Width) {widthSpinBox->setValue(Width);}
    void setPageHeight(double Height) {heightSpinBox->setValue(Height);}
    void setMarginTop(double Height){topSpinBox->setValue(Height);}
    void setMarginLeft(double Width){leftSpinBox->setValue(Width);}
    void setMarginRight(double Width){rightSpinBox->setValue(Width);}
    void setMarginBottom(double Height){bottomSpinBox->setValue(Height);}
};

class PageLayoutView: public QWidget
{
    Q_OBJECT

public:
    PageLayoutView(QWidget *parent = 0);

private:
    double PageWidth;
    double PageHeight;
    double MarginTop;
    double MarginLeft;
    double MarginRight;
    double MarginBottom;
    //DirectionSelector *ds;

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void setPageWidth(double Width);
    void setPageHeight(double Height);
    void setMarginTop(double Height);
    void setMarginLeft(double Width);
    void setMarginRight(double Width);
    void setMarginBottom(double Height);
};

#endif
