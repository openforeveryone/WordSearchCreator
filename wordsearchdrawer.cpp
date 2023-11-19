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

#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QSvgGenerator>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include <QClipboard>

#include "wordsearchdrawer.h"

WordSearchDrawer::WordSearchDrawer(WordSearchDoc *doc, QWidget *parent)
    : QWidget(parent)
{
    this->doc = doc;
    setAttribute(Qt::WA_NoSystemBackground);
    //setAttribute(Qt::WA_PaintOnScreen);
    setAutoFillBackground(false);
    //setBackgroundRole(Qt::white);
    viewMode=true;
    scale = 1;
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    answering = false;
    connect(doc, SIGNAL(EditedStateChanged(bool)), this, SLOT(update()));
}

void WordSearchDrawer::paintEvent(QPaintEvent *event)
{
    event->accept();
    stopwatch.start();
    QPainter painter(this);
    //painter.save();
    painter.scale(scale,scale);
    if (viewMode) {
        painter.fillRect (0, 0, width()/scale+1, height()/scale+1, QColor(200,200,200));
        painter.translate(20,20);
        painter.fillRect (3, 3, painter.device()->logicalDpiX() * doc->pagewidth, painter.device()->logicalDpiY() * doc->pageheight, QColor(0, 0, 0));
        painter.fillRect (0, 0, painter.device()->logicalDpiX() * doc->pagewidth, painter.device()->logicalDpiY() * doc->pageheight, doc->getBGColor());
        painter.drawRect (0, 0, painter.device()->logicalDpiX() * doc->pagewidth, painter.device()->logicalDpiY() * doc->pageheight);
        drawWS(&painter, 0, 0, logicalDpiX() * doc->pagewidth - 0, logicalDpiY() * doc->pageheight + 0);
    }
    else {
        painter.setFont(doc->getWSFont());
        boxsize = painter.fontMetrics().height();
        int WordSearchPixelwidth = (boxsize * doc->ws->XSize());
        int WordSearchPixelheight = (boxsize * doc->ws->YSize());
        int xo = (width()/scale-WordSearchPixelwidth)/2,yo = (height()/scale-WordSearchPixelheight)/2;
        wspwidth = WordSearchPixelwidth;
        wspheight = WordSearchPixelheight;
        Wordsarchpos = QPoint(xo,yo);
        painter.fillRect (0, 0, width()/scale+1, height()/scale+1, doc->getBGColor());
        painter.translate(xo,yo);
        drawWordSearch(&painter);
    }
    setMinimumSize(sizeHint());
}

void WordSearchDrawer::drawWS(QPainter *painter, int x, int y, int w, int h)
{
    int xo, yo;
    painter->setFont(doc->getWSFont());
    int boxsize = painter->fontMetrics().height();
    int WordSearchPixelwidth = (boxsize * doc->ws->XSize())+1;
    int WordSearchPixelheight = (boxsize * doc->ws->YSize())+1;
    int DpiX=painter->device()->logicalDpiX();
    int DpiY=painter->device()->logicalDpiY();

    //margins

    painter->translate(DpiX*doc->leftMargin,DpiY*doc->topMargin);
    w -= DpiX*doc->leftMargin + DpiX*doc->rightMargin;
    h -= DpiY*doc->topMargin + DpiY*doc->bottomMargin;

    xo = ((w-WordSearchPixelwidth)/2)+x;
    yo = y;
    if (doc->getTitle()!="")
    {
        painter->setFont(doc->getTitleFont());
        painter->setPen(doc->getTitleColor());
        painter->drawText(0, yo, w, painter->fontMetrics().height(), Qt::AlignCenter , doc->getTitle());
        yo += painter->fontMetrics().height()*1.2;
    }
    if (doc->getShowWords() && doc->numwordsplaced())
    {
        painter->setPen(doc->getWLColor());
        QFont WordsFont  = doc->getWordsFont();
        painter->setFont(doc->getWordsFont());
        int wordspercol = ((h-(yo+WordSearchPixelheight)) / painter->fontMetrics().height())-1;
        if (wordspercol > 0)
        {
            uint colsneded = ceil(float (doc->numwordsplaced()) / float(wordspercol));
            int colwidth = 0;
            for (uint a = 0; a < doc->ws->wordlist.size(); a++)
            {
                if (doc->ws->wordlist.at(a).used)
                    colwidth=qMax(colwidth,  painter->fontMetrics().width(doc->ws->wordlist.at(a).word));
            }
            colwidth += colwidth * 0.2;
            uint maxcols = w / colwidth;
            uint numcols=colsneded, bestspaceleft = 0;
            for (uint a=colsneded; a < maxcols; a++)
            {
                uint spaceleft = ceil(float(doc->numwordsplaced())/float(a))*a-doc->numwordsplaced();
                if (!(doc->numwordsplaced()%a))
                    spaceleft = 0;
                if (spaceleft < bestspaceleft)
                {
                    numcols = a;
                    bestspaceleft = spaceleft;
                }
            }
            uint x = 0, y = 0;
            int a = 0;
//            doc->sortedwordlist.clear();
//            while (a < doc->ws->wordlist.size())
//            {
//                doc->sortedwordlist.append(doc->ws->wordlist.at(a));
//                a++;
//            }
//            a=0;
//            if (doc->wordlistorder==WordSearchDoc::Alphabetical)
//                while (a < doc->sortedwordlist.size()-1)
//                {
//                if ( QString::localeAwareCompare(doc->sortedwordlist.at(a).word,doc->sortedwordlist.at(a+1).word)>0)
//                {
//                    doc->sortedwordlist.move(a+1, a);
//                    a=0;
//                }
//                else
//                    a++;
//            }
//            if (doc->wordlistorder==WordSearchDoc::UserDefined)
//                while (a < doc->sortedwordlist.size()-1)
//                {
//                if ( doc->sortedwordlist.at(a).listpos > doc->sortedwordlist.at(a+1).listpos)
//                {
//                    doc->sortedwordlist.move(a+1, a);
//                    a=0;
//                }
//                else
//                    a++;
//            }
//            if (doc->wordlistorder==WordSearchDoc::ReverseAlphabetical)
//                while (a < doc->sortedwordlist.size()-1)
//                {
//                if ( QString::localeAwareCompare(doc->sortedwordlist.at(a).word,doc->sortedwordlist.at(a+1).word)<0)
//                {
//                    doc->sortedwordlist.move(a+1, a);
//                    a=0;
//                }
//                else
//                    a++;
//            }
//            if (doc->wordlistorder==WordSearchDoc::SizeDes)
//                while (a < doc->sortedwordlist.size()-1)
//                {
//                if (doc->sortedwordlist.at(a).word.size()>doc->sortedwordlist.at(a+1).word.size())
//                {
//                    doc->sortedwordlist.move(a+1, a);
//                    a=0;
//                }
//                else
//                    a++;
//            }
            a=0;            
            QList<Word> sortedwordlist = doc->sortedWordList();
            while (a < sortedwordlist.size())
            {
                if (sortedwordlist.at(a).used)
                {
                    if (sortedwordlist.at(a).answered==true)
                        WordsFont.setStrikeOut(true);
                    else
                        WordsFont.setStrikeOut(false);
                    painter->setFont(WordsFont);
                    painter->drawText((w-colwidth*numcols)/2+colwidth*x, (yo+WordSearchPixelheight)+( painter->fontMetrics().height()*(y+1)), colwidth, painter->fontMetrics().height(), Qt::AlignCenter, sortedwordlist.at(a).word);
                    if (y < ceil(float(doc->numwordsplaced())/float(numcols))-1)
                        y++;
                    else
                    {
                        y = 0;
                        x++;
                    }
                }
                a++;
            }
        }
    }
    if (doc->getShowCreatorLabel())
    {
        painter->setFont(doc->getCLFont());
        QString tempfooter = doc->getFooter();
        if (tempfooter.isNull())
            tempfooter = "Created with Word Search Creator from: WordSearchCreator.org";
        painter->drawText(0, h , w, painter->fontMetrics().height(), Qt::AlignRight, tempfooter);
    }
    Wordsarchpos = QPoint (xo+20+DpiX*doc->leftMargin,yo+20+DpiY*doc->topMargin);
    painter->translate(xo,yo);
    drawWordSearch(painter);
}

void WordSearchDrawer::setAutoResize(int autosize)
{
    doc->setAutoResize(autosize);
}

QSize WordSearchDrawer::sizeHint() const
{
    if (!viewMode)
        return QSize(wspwidth,wspheight)*scale;
    else return QSize(logicalDpiX() * doc->pagewidth + 50, logicalDpiY() * doc->pageheight + 50)*scale;
}

void WordSearchDrawer::setScale(double scale)
{
    if (this->scale!=scale)
    {
        this->scale=scale;
        emit scaleChanged(scale);
        update();
    }
}

void WordSearchDrawer::zoomIn()
{
    setScale(scale + .1);
}

void WordSearchDrawer::zoomOut()
{
    if (scale > .1)
        setScale(scale - .1);
}

void WordSearchDrawer::autoZoom()
{
    setScale(scale*(double (parentWidget()->width())/double(sizeHint().width())));
}

void WordSearchDrawer::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    QPoint wspoint = QPoint(floor((point/scale - Wordsarchpos).x()/float(boxsize)),floor((point/scale - Wordsarchpos).y()/float(boxsize)));
    if (!((wspoint.x()<0) || (wspoint.y()<0) || (wspoint.x() >= doc->ws->XSize()) || (wspoint.y() >= doc->ws->YSize())))
    {
        answerend=wspoint;
        update();
        if (answerstart != answerend)
            mousemoved = true;
    }
}

void WordSearchDrawer::mousePressEvent(QMouseEvent *event)
{
    if (!answering)
    {
        QPoint point = event->pos();
        answerstart = QPoint(floor((point/scale - Wordsarchpos).x()/float(boxsize)),floor((point/scale - Wordsarchpos).y()/float(boxsize)));
        answerend = answerstart;
        if (!((answerstart.x()<0) || (answerstart.y()<0) || (answerstart.x() >= doc->ws->XSize()) || (answerstart.y() >= doc->ws->YSize())))
        {
            answering = true;
            mousemoved=false;
            update();
        }
    }
}

void WordSearchDrawer::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    if (mousemoved)
    {
        setMouseTracking(false);
        answering = false;
        //check answer
        //needs improving
        uint i =0;
        while (i < doc->ws->wordlist.size())
        {
            if (doc->ws->wordlist[i].used!=0)
            {
                int x1,y1,x2,y2;
                x1 = doc->ws->wordlist[i].xpos-1;
                y1 = doc->ws->wordlist[i].ypos-1;
                x2 = doc->ws->wordlist[i].expos-1;
                y2 = doc->ws->wordlist[i].eypos-1;
                if ((answerstart.x() == x1) && (answerstart.y() == y1) && (answerend.x() == x2) && (answerend.y() == y2))
                {
                    doc->ws->wordlist[i].answered=true;
                    emit someAnswerdChanged(true);
                    doc->setEditedState(true);
                }
            }
            i++;
        }
    }
    else
        setMouseTracking(true);
    repaint();
}

void WordSearchDrawer::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        if (event->delta()>0)
            zoomIn();
        else
            zoomOut();
        event->accept();
    }
    else
        event->ignore();
}

void WordSearchDrawer::clearPlayAnswers()
{
    for (uint i = 0; i < doc->ws->wordlist.size(); i++)
    {
        doc->ws->wordlist[i].answered=false;
    }
    emit someAnswerdChanged(false);
    repaint();
}

void WordSearchDrawer::Print()
{
    QPrinter printer;
    printer.setFullPage(true);
    if (doc->qpps != QPrinter::Custom)
    {
        printer.setPageSize(doc->qpps);
        if (doc->pagewidth>doc->pageheight)
            printer.setOrientation(QPrinter::Landscape);
    }
    else
        printer.setPaperSize(QSizeF(doc->pagewidth, doc->pageheight),QPrinter::Inch);
    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        drawWS(&painter, 0, 0, printer.width(), printer.height());
    }
}

void WordSearchDrawer::DPrint()
{
    QPrinter printer;
    printer.setFullPage(true);
    if (doc->qpps != QPrinter::Custom)
    {
        printer.setPageSize(doc->qpps);
        if (doc->pagewidth>doc->pageheight)
            printer.setOrientation(QPrinter::Landscape);
    }
    else
        printer.setPaperSize(QSizeF(doc->pagewidth, doc->pageheight),QPrinter::Inch);
    QPainter painter(&printer);
    drawWS(&painter, 0, 0, printer.width(), printer.height());
}

void WordSearchDrawer::Copy()
{
    QImage aimage(1,1,QImage::Format_ARGB32);
    QPainter apainter(&aimage);
    apainter.setFont(doc->getWSFont());
    int boxsize = apainter.fontMetrics().height();
    int WordSearchPixelwidth = (boxsize * doc->ws->XSize())+1;
    int WordSearchPixelheight = (boxsize * doc->ws->YSize())+1;
    QPixmap wsimage(WordSearchPixelwidth,WordSearchPixelheight);
    QPainter painter(&wsimage);
    painter.fillRect (0, 0, wsimage.width(), wsimage.height(), QColor(255,255,255));
    drawWordSearch(&painter);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(wsimage);
}


void WordSearchDrawer::PDF()
{
    QString path = QDir::homePath();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export as PDF.."),
                                                    path,
                                                    tr("PDF files (*.pdf)"));
    if (fileName.isEmpty())
        return;
    if (fileName.mid(fileName.size()-4,4) != ".pdf")
        fileName = fileName + ".pdf";
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setFullPage(true);
    if (doc->qpps != QPrinter::Custom)
    {
        printer.setPageSize(doc->qpps);
        if (doc->pagewidth>doc->pageheight)
            printer.setOrientation(QPrinter::Landscape);
    }
    else
        printer.setPaperSize(QSizeF(doc->pagewidth, doc->pageheight),QPrinter::Inch);
    QPainter painter(&printer);
    //printer.setPageSize(QPrinter::A4);
    drawWS(&painter, 0, 0, printer.width(),printer.height());
}

void WordSearchDrawer::drawWordSearch(QPainter *painter)
{
    QChar letter;
    int xsize = doc->ws->XSize();
    int ysize = doc->ws->YSize();
    uint i;
    double DpiX=painter->device()->logicalDpiX();
    painter->setFont(doc->getWSFont());
    boxsize = painter->fontMetrics().height();
    QPen boxpen;
    boxpen.setColor(doc->getGridColor());
    double gridPixleWidth = doc->getGridWidth()*DpiX;
    if (gridPixleWidth*scale < 2)
        gridPixleWidth = 0;
    boxpen.setWidth(gridPixleWidth);
    QPen wsPen;
    wsPen.setColor(doc->getWSColor());
    for (int x = 0; x < xsize; x++)
    {
        for (int y = 0; y < ysize; y++)
        {

            letter = doc->ws->Letter(x,y);
            painter->setPen(wsPen);
            if (doc->ws->Mask(x+1,y+1))
            {
                painter->drawText((x * boxsize), (y * boxsize), boxsize ,boxsize ,Qt::AlignCenter , letter);
                if (doc->getShowGrid())
                {
                    painter->setPen(boxpen);
                    painter->drawRect ((x * boxsize), (y * boxsize), boxsize, boxsize);
                }
            }
        }
    }
    QPen linepen;
    QColor linecolour(doc->getHLColor());
    linecolour.setAlpha(150);
    linepen.setColor(linecolour);
    linepen.setWidth(boxsize/2);
    linepen.setCapStyle(Qt::RoundCap);
    painter->setPen(linepen);
    i = 0;
    while (i < doc->ws->wordlist.size())
    {
        if ((doc->getShowAnswers() && doc->ws->wordlist[i].used!=0) || doc->ws->wordlist[i].answered==true)
        {
            int x1,y1,x2,y2;
            x1 = doc->ws->wordlist[i].xpos-1;
            y1 = doc->ws->wordlist[i].ypos-1;
            x2 = doc->ws->wordlist[i].expos-1;
            y2 = doc->ws->wordlist[i].eypos-1;
            painter->drawLine(((boxsize/2)+boxsize*x1),((boxsize/2)+boxsize*y1),((boxsize/2)+boxsize*x2),((boxsize/2)+boxsize*y2));
            if ((x1 == x2) && (y1 == y2))
                painter->drawPoint(((boxsize/2)+boxsize*x1),((boxsize/2)+boxsize*y1));
        }
        i++;
    }
    if (answering)
    {
        QPen linepen;
        QColor linecolour(0,0,255,150);
        linepen.setColor(linecolour);
        linepen.setWidth(boxsize/2);
        linepen.setCapStyle(Qt::RoundCap);
        painter->setPen(linepen);
        int x1,y1,x2,y2;
        x1 = answerstart.x();
        y1 = answerstart.y();
        x2 = answerend.x();
        y2 = answerend.y();
        painter->drawLine(((boxsize/2)+boxsize*x1),((boxsize/2)+boxsize*y1),((boxsize/2)+boxsize*x2),((boxsize/2)+boxsize*y2));
        if ((x1 == x2) && (y1 == y2))
            painter->drawPoint(((boxsize/2)+boxsize*x1),((boxsize/2)+boxsize*y1));
    }
}

void WordSearchDrawer::SVG()
{
    QString path = QDir::homePath();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export as SVG.."),
                                                    path,
                                                    tr("SVG files (*.svg)"));
    if (fileName.isEmpty())
        return;
    if (fileName.mid(fileName.size()-4,4) != ".svg")
        fileName = fileName + ".svg";
//    QPrinter printer;
    QSvgGenerator svg;
    svg.setFileName(fileName);
    QPainter painter(&svg);
    drawWordSearch(&painter);
}
