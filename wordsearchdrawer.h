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

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);

private:
    bool viewMode;
    void drawWS(QPainter *painter, int x, int y, int w, int h);
    void drawWordSearch(QPainter *painter);
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
//    QTime stopwatch;
//    bool firstpaint;

public slots:
    void Print();
    void DPrint();
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
