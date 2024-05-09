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

#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QSvgGenerator>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include <QClipboard>
#include <QDebug>

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
    painter.scale(scale,scale);
    if (viewMode) {
        calcPageCount(&painter);
        //Draw the backgrounds and page outline
        painter.fillRect (0, 0, width()/scale+1, height()/scale+1, QColor(200,200,200));
        for (int page = 1; page <= getPageCount(); page++)
        {
            painter.save();
            painter.translate(20, (logicalDpiY() * doc->pageHeight + 25) * (page - 1) + 20);
            painter.fillRect (3, 3, painter.device()->logicalDpiX() * doc->pageWidth, painter.device()->logicalDpiY() * doc->pageHeight, QColor(0, 0, 0));
            painter.fillRect (0, 0, painter.device()->logicalDpiX() * doc->pageWidth, painter.device()->logicalDpiY() * doc->pageHeight, doc->getBGColor());
            QPen pen;
            pen.setWidth(0);
            painter.setPen(pen);
            painter.drawRect (0, 0, painter.device()->logicalDpiX() * doc->pageWidth, painter.device()->logicalDpiY() * doc->pageHeight);

            drawWorksheetPage(&painter, logicalDpiX() * doc->pageWidth - 0, logicalDpiY() * doc->pageHeight + 0, page, true);
            painter.restore();
        }
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
        drawWordSearch(&painter, true);
    }
    setMinimumSize(sizeHint());
}

void WordSearchDrawer::calcPageCount(QPainter *painter)
{
    int w = logicalDpiX() * doc->pageWidth;
    int h = logicalDpiY() * doc->pageHeight;
    int DpiX=painter->device()->logicalDpiX();
    int DpiY=painter->device()->logicalDpiY();

    w -= DpiX*doc->leftMargin + DpiX*doc->rightMargin;
    h -= DpiY*doc->topMargin + DpiY*doc->bottomMargin;

    QFont WordsFont = doc->getWordsFont();
    painter->setFont(WordsFont);
    int lineHeight = painter->fontMetrics().height();
    painter->setFont(doc->getWSFont());
    int boxsize = painter->fontMetrics().height();
    int WordSearchPixelHeight = (boxsize * doc->ws->YSize())+1;
    painter->setFont(doc->getTitleFont());
    int yo;
    pageCount = 1;
    firstWordOnPage.clear();
    uint wordsOnPreviousPages = 0;
    if (doc->numwordsplaced() == 0)
        return;
    while (true)
    {
        if (pageCount == 1)
        {
            yo = painter->fontMetrics().height()*1.2;
            yo += WordSearchPixelHeight;
            painter->setFont(WordsFont);
        }
        else { yo = 0; }
        int spaceHeightInLines = ((h-(yo)) / lineHeight)-1;
        if (spaceHeightInLines < 1)
        {
            firstWordOnPage.append(wordsOnPreviousPages);
            pageCount++;
            continue;
        }
        int colWidth = 0;
        for (uint a = 0; a < doc->ws->wordlist.size(); a++)
        {
            if (doc->ws->wordlist.at(a).used)
                colWidth = qMax(colWidth, painter->fontMetrics().width(doc->ws->wordlist.at(a).word));
        }
        colWidth += colWidth * 0.2;
        uint maxCols = w / colWidth;
        uint maxWordsOnThisPage = spaceHeightInLines * maxCols;
        if (maxWordsOnThisPage >= doc->numwordsplaced() - wordsOnPreviousPages)
        {
            firstWordOnPage.append(doc->numwordsplaced() - wordsOnPreviousPages);
            return;
        }
        wordsOnPreviousPages += maxWordsOnThisPage;
        firstWordOnPage.append(wordsOnPreviousPages);
        pageCount++;
    }
}

void WordSearchDrawer::drawWorksheetPage(QPainter *painter, int w, int h, int pageNumber, bool screen)
{
    int xo, yo;
    painter->setFont(doc->getWSFont());
    int boxsize = painter->fontMetrics().height();
    int WordSearchPixelWidth = (boxsize * doc->ws->XSize())+1;
    int WordSearchPixelHeight = (boxsize * doc->ws->YSize())+1;
    int DpiX=painter->device()->logicalDpiX();
    int DpiY=painter->device()->logicalDpiY();

    //margins

    painter->translate(DpiX*doc->leftMargin, DpiY*doc->topMargin);
    w -= DpiX*doc->leftMargin + DpiX*doc->rightMargin;
    h -= DpiY*doc->topMargin + DpiY*doc->bottomMargin;

    xo = ((w-WordSearchPixelWidth)/2);
    yo = 0;
    if (doc->getTitle()!="" && pageNumber==1)
    {
        painter->setFont(doc->getTitleFont());
        painter->setPen(doc->getTitleColor());
        painter->drawText(0, yo, w, painter->fontMetrics().height(), Qt::AlignCenter , doc->getTitle());
        yo += painter->fontMetrics().height()*1.2;
    }    
    if (pageNumber==1)
    {
        painter->save();
        painter->translate(xo, yo);
        drawWordSearch(painter, screen);
        Wordsarchpos = QPoint (xo+20+DpiX*doc->leftMargin, yo+20+DpiY*doc->topMargin);
        painter->restore();
        yo += WordSearchPixelHeight;
    }
    if (doc->getShowWords() && doc->numwordsplaced())
    {
        painter->setPen(doc->getWLColor());
        QFont WordsFont  = doc->getWordsFont();
        painter->setFont(WordsFont);
        int spaceHeightInLines = ((h-(yo)) / painter->fontMetrics().height())-1;
        if (spaceHeightInLines > 0)
        {
            // Determine word list layout
            int colWidth = 0;
            for (uint a = 0; a < doc->ws->wordlist.size(); a++)
            {
                if (doc->ws->wordlist.at(a).used)
                    colWidth = qMax(colWidth,  painter->fontMetrics().width(doc->ws->wordlist.at(a).word));
            }
            colWidth += colWidth * 0.2;
            uint maxCols = w / colWidth;
            uint numcols;
            if (pageCount == 1)
            {
                // Find aesthetically pleasing number of columns by searching for the number of columns
                // that leaves the least space in the last column.
                uint minColsNeded = ceil(float (doc->numwordsplaced()) / float(spaceHeightInLines));
                numcols = minColsNeded;
                uint bestspaceleft = 0;
                for (uint a=minColsNeded; a < maxCols; a++)
                {
                    uint spaceLeft = ceil(float(doc->numwordsplaced()) / float(a)) * a - doc->numwordsplaced();
                    if (!(doc->numwordsplaced() % a))
                        spaceLeft = 0;
                    if (spaceLeft < bestspaceleft)
                    {
                        numcols = a;
                        bestspaceleft = spaceLeft;
                    }
                }
            }
            else
            {
                numcols = maxCols;
            }

            // Draw word list
            uint x = 0, y = 0;
            uint firstWordOnThisPage = 0;
            if (pageNumber > 1)
                firstWordOnThisPage = firstWordOnPage.at(pageNumber-2);
            QList<Word> wordList = doc->sortedUsedWordList();
            int numWordsOnPage = min(maxCols * spaceHeightInLines, wordList.size() - firstWordOnThisPage);
            for (uint a = firstWordOnThisPage; a < firstWordOnThisPage + numWordsOnPage; a++)
            {
                if (wordList.at(a).answered==true)
                    WordsFont.setStrikeOut(true);
                else
                    WordsFont.setStrikeOut(false);
                painter->setFont(WordsFont);
                painter->drawText((w-colWidth*numcols)/2+colWidth*x, (yo)+( painter->fontMetrics().height()*(y+1)),
                                  colWidth, painter->fontMetrics().height(), Qt::AlignCenter, wordList.at(a).word);
                if (y < ceil(float(numWordsOnPage)/float(numcols))-1)
                    y++;
                else
                {
                    y = 0;
                    x++;
                }
            }
        }
    }
    if (getPageCount() > 1)
    {
        painter->setFont(doc->getCLFont());
        QString pageNumberFooter = QString("Page %1 of %2").arg(pageNumber).arg(getPageCount());
        painter->drawText(0, h , w, painter->fontMetrics().height(), Qt::AlignLeft, pageNumberFooter);
    }
    if (doc->getShowCreatorLabel())
    {
        painter->setFont(doc->getCLFont());
        QString tempfooter = doc->getFooter();
        if (tempfooter.isNull())
            tempfooter = "Created with Word Search Creator from: WordSearchCreator.org";
        painter->drawText(0, h , w, painter->fontMetrics().height(), Qt::AlignRight, tempfooter);
    }
}

void WordSearchDrawer::setAutoResize(int autosize)
{
    doc->setAutoResize(autosize);
}

QSize WordSearchDrawer::sizeHint() const
{
    if (!viewMode)
        return QSize(wspwidth,wspheight)*scale;
    else return QSize(logicalDpiX() * doc->pageWidth + 50, (logicalDpiY() * doc->pageHeight + 25) * getPageCount() + 25) * scale;
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
    bool changed = false;
    for (uint i = 0; i < doc->ws->wordlist.size(); i++)
    {
        if (doc->ws->wordlist[i].answered) changed = true;
        doc->ws->wordlist[i].answered=false;
    }
    if (changed)
    {
        doc->setEditedState(true);
        emit someAnswerdChanged(false);
        repaint();
    }
}

void WordSearchDrawer::print(QPrinter *printer)
{
    QPainter painter(printer);
    for (int page = 1; page <= getPageCount(); page++)
    {
        painter.save();
        drawWorksheetPage(&painter, printer->width(), printer->height(), page, false);
        painter.restore();
        if (page < getPageCount())
            printer->newPage();
    }
}

void WordSearchDrawer::printWithDialog()
{
    QPrinter printer;
    printer.setFullPage(true);
    if (doc->qpps != QPrinter::Custom)
    {
        printer.setPageSize(doc->qpps);
        if (doc->pageWidth>doc->pageHeight)
            printer.setOrientation(QPrinter::Landscape);
    }
    else
        printer.setPaperSize(QSizeF(doc->pageWidth, doc->pageHeight),QPrinter::Inch);
    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() == QDialog::Accepted) {
        print(&printer);
    }
}

void WordSearchDrawer::printDirect()
{
    QPrinter printer;
    printer.setFullPage(true);
    if (doc->qpps != QPrinter::Custom)
    {
        printer.setPageSize(doc->qpps);
        if (doc->pageWidth>doc->pageHeight)
            printer.setOrientation(QPrinter::Landscape);
    }
    else
        printer.setPaperSize(QSizeF(doc->pageWidth, doc->pageHeight),QPrinter::Inch);
    print(&printer);
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
    drawWordSearch(&painter, true);
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
        if (doc->pageWidth>doc->pageHeight)
            printer.setOrientation(QPrinter::Landscape);
    }
    else
        printer.setPaperSize(QSizeF(doc->pageWidth, doc->pageHeight),QPrinter::Inch);
    print(&printer);
}

void WordSearchDrawer::drawWordSearch(QPainter *painter, bool screen)
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
    qreal gridPixleWidth;
    if (screen)
    {
        gridPixleWidth = doc->getGridWidth() * DpiX;
        if (gridPixleWidth*scale < 2)
            gridPixleWidth = 0;
    } else {
        gridPixleWidth = qMax(doc->getGridWidth(), 1.0/300.0) * DpiX;
    }
    boxpen.setWidthF(gridPixleWidth);
    boxpen.setJoinStyle(Qt::MiterJoin);
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
    if (doc->getHLSolid())
    {
        linecolour.setAlpha(150);
        linepen.setWidth(boxsize/2);
        linepen.setCapStyle(Qt::RoundCap);
    }
    else
    {
        linecolour.setAlpha(255);
        linepen.setWidth(boxsize/20);
        linepen.setCapStyle(Qt::FlatCap);
    }
    linepen.setColor(linecolour);
    painter->setPen(linepen);
    painter->setRenderHint(QPainter::Antialiasing);
    i = 0;
    while (i < doc->ws->wordlist.size())
    {
        if ((doc->getShowAnswers() && doc->ws->wordlist[i].used!=0) || doc->ws->wordlist[i].answered==true)
        {
            int x1, y1, x2, y2;
            x1 = doc->ws->wordlist[i].xpos-1;
            y1 = doc->ws->wordlist[i].ypos-1;
            x2 = doc->ws->wordlist[i].expos-1;
            y2 = doc->ws->wordlist[i].eypos-1;
            drawAnswer(painter, x1, y1, x2, y2);
        }
        i++;
    }
    if (answering)
    {
        int x1, y1, x2, y2;
        x1 = answerstart.x();
        y1 = answerstart.y();
        x2 = answerend.x();
        y2 = answerend.y();
        drawAnswer(painter, x1, y1, x2, y2);
    }
    painter->setRenderHint(QPainter::Antialiasing, false);
}

void WordSearchDrawer::drawAnswer(QPainter *painter, int x1, int y1, int x2, int y2)
{
    qreal boxsize = this->boxsize;
    QPointF start(boxsize/2+boxsize*qreal(x1), boxsize/2+boxsize*qreal(y1));
    QPointF end(boxsize/2+boxsize*qreal(x2), boxsize/2+boxsize*qreal(y2));
    if (!doc->getHLSolid()){
        qreal answerWidth_2 = 0.4;
        qreal absAnswerWidth_2 = boxsize*answerWidth_2;
        qreal angleRad = qAtan2(y2-y1, x2-x1);
        qreal angle = angleRad / M_PI * 180;
        QRectF startArcBoundingRect((boxsize/2-absAnswerWidth_2)+boxsize*x1, (boxsize/2-absAnswerWidth_2)+boxsize*y1, absAnswerWidth_2*2, absAnswerWidth_2*2);
        QRectF endArcBoundingRect((boxsize/2-absAnswerWidth_2)+boxsize*x2, (boxsize/2-absAnswerWidth_2)+boxsize*y2, absAnswerWidth_2*2, absAnswerWidth_2*2);
        painter->drawArc(startArcBoundingRect, (-angle+90)*16, 180*16);
        painter->drawArc(endArcBoundingRect, (-angle-90)*16, 180*16);
        painter->drawLine(QLineF(start.x() + cos(angleRad-M_PI_2)*absAnswerWidth_2, start.y() + sin(angleRad-M_PI_2)*absAnswerWidth_2,
                                 end.x() + cos(angleRad-M_PI_2)*absAnswerWidth_2, end.y() + sin(angleRad-M_PI_2)*absAnswerWidth_2));
        painter->drawLine(QLineF(start.x() + cos(angleRad+M_PI_2)*absAnswerWidth_2, start.y() + sin(angleRad+M_PI_2)*absAnswerWidth_2,
                                 end.x() + cos(angleRad+M_PI_2)*absAnswerWidth_2, end.y() + sin(angleRad+M_PI_2)*absAnswerWidth_2));
    }
    else
    {
        painter->drawLine(start, end);
        if ((x1 == x2) && (y1 == y2))
            painter->drawPoint(start);
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
    drawWordSearch(&painter, false);
}
