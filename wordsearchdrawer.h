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

#ifndef WORDSEARCHDRAWER_H
#define WORDSEARCHDRAWER_H

#include <QWidget>
#include <QTime>

#include "wordsearchdoc.h"

class WordSearchDrawer : public QWidget
{
    Q_OBJECT

public:
    WordSearchDrawer(WordSearchDoc *doc, QWidget *parent = 0);
    WordSearchDoc *doc;
    QSize sizeHint() const;
    int getPageCount() const {return pageCount;}

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);

private:
    bool viewMode;
    void drawWorksheetPage(QPainter *painter, int w, int h, int pageNumber, bool screen);
    void drawWordSearch(QPainter *painter, bool screen);
    void drawAnswer(QPainter *painter, int x1, int y1, int x2, int y2);
    void calcPageCount(QPainter *painter);
    double scale;
    int wspwidth;
    int wspheight;
    QPoint answerstart;
    QPoint answerend;
    QPoint Wordsarchpos;
    int boxsize;
    bool answering;
    bool mousemoved;
    bool allowPlay;
    QTime stopwatch;
    int pageCount;
    QList<uint> firstWordOnPage;

public slots:
    void print(QPrinter *printer);
    void printWithDialog();
    void printDirect();
    void PDF();
    void setAutoResize(int autosize);
    bool ShowAnswers() {return doc->ShowAnswers();}
    void setScale(double scale);
    void zoomIn();
    void zoomOut();
    void autoZoom();
    void Copy();
    void pageLayout() {this->viewMode = true; update();}
    void screenLayout() {this->viewMode = false; update();}
    void setAllowPlay(bool allow) {this->allowPlay = allow; update();}
    void clearPlayAnswers();
    void SVG();

signals:
    void EditedStateChanged(bool edited);
    void scaleChanged(double scale);
    void useMaskChanged(int useMask);
    void someAnswerdChanged(bool answerMode);
};

#endif
