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

#ifndef WORDSEARCHDOC_H
#define WORDSEARCHDOC_H

#include <QtPrintSupport/QPrinter>
#include <QFont>
#include <QColor>
#include <QIODevice>

#include "wordsearch.h"

class WordSearchDoc : public QObject
{
    Q_OBJECT

public:
    WordSearchDoc(QWidget *parent = 0);
    ~WordSearchDoc();
    wordsearch *ws;
    bool getShowAnswers();
    bool getShowWords();
    bool getShowGrid() {return showGrid;}
    bool getMaskChanged() {return maskChanged;}
    bool getUseMask() {return useMask;}
    QFont getWSFont();
    QFont getTitleFont();
    QFont getWordsFont();
    QFont getCLFont();
    QColor getBGColor() {return bgColor;}
    QColor getHLColor() {return hlColor;}    
    bool getHLSolid() {return hlSolid;}
    QColor getWSColor() {return wsColor;}
    QColor getTitleColor() {return titleColor;}
    QColor getWLColor() {return wlColor;}
    QColor getGridColor() {return gridColor;}
    double getGridWidth() {return gridWidth;}
    bool getEdited() {return edited;}
    bool getShowCreatorLabel() {return showCreatorLabel;}
    QString getTitle();
    uint numwordsplaced();
    double topMargin, leftMargin, rightMargin, bottomMargin;
    double pageWidth;
    double pageHeight;
    QPrinter::PageSize qpps;
    int Open(QString fileName);
    bool saveToIO(QIODevice &file);
    QString filename;
    void Create();
    void drawWS(QPainter *painter, int x, int y, int w, int h);
    void drawWordSearch(QPainter *painter);
    QString getFooter() {return footer;}
    //QList<Word> sortedwordlist;
    enum ordertype {UserDefined, Alphabetical, ReverseAlphabetical, SizeAsc, SizeDes};
    ordertype wordlistorder;
    void setWordSpace (wordsearch::wordSpaceType wordSpace) {ws->wordSpace=wordSpace;}
    void clearAnswers();
    QList<Word> sortedUsedWordList();
    QList<Word> fullWordList();
    enum showPageNumberType {PageNumberNever, PageNumberAlways, PageNumberMultipage};
    showPageNumberType getShowPageNumber() {return showPageNumber;}
    bool getShowPageCount() {return showPageCount;}
    static QString getDefaultFooter() {return "Created with Word Search Creator from: WordSearchCreator.org";}

private:
    QFont WSFont;
    QFont TitleFont;
    QFont WordsFont;
    QColor bgColor;
    QColor hlColor;
    bool hlSolid;
    QColor titleColor;
    QColor wsColor;
    QColor wlColor;
    QColor gridColor;
    double gridWidth;
    QFont creatorLabelFont;
    bool showanswers;
    bool showwords;
    bool showGrid;
    bool maskChanged;
    bool useMask;
    bool viewMode;
    QString title;
    bool showCreatorLabel;
    bool **mask;
    bool edited;
    int sortmethod;
    int boxsize;
    QString footer;
    showPageNumberType showPageNumber;
    bool showPageCount;

public slots:
    void setWSFont(QFont WSFont);
    void setWSFont();
    void setTitleFont(QFont WSFont);
    void setTitleFont();
    void setWordsFont(QFont WordsFont);
    void setWordsFont();
    void setTitleColor(QColor titleColor) {this->titleColor = titleColor;}
    void setWSColor(QColor wsColor) {this->wsColor = wsColor;}
    void setWLColor(QColor wlColor) {this->wlColor = wlColor;}
    void setShowGrid(bool showGrid) {this->showGrid = showGrid;}
    void setMaskChanged(bool maskChanged) {this->maskChanged = maskChanged;}
    void setUseMask(int useMask) {this->useMask = useMask; emit useMaskChanged(useMask);}
    void setWordsColor(QColor wlColor) {this->wlColor = wlColor;}
    void setBgColor(QColor bgColor) {this->bgColor = bgColor;}
    void setHlColor(QColor hlColor) {this->hlColor = hlColor;}
    void setHlSolid(bool hlSolid) {this->hlSolid = hlSolid;}
    void setGridColor(QColor gridColor) {this->gridColor = gridColor;}
    void setGridWidth(double gridWidth) {this->gridWidth = gridWidth;}
    void setSize(int x, int y);
    void setXSize(int x);
    void setYSize(int y);
    void setAutoResize(int autosize);
    bool ShowAnswers() {return showanswers;}
    void setShowAnswers(int showanswers);
    void setShowWords(int showwords);
    void setTitle(QString Title);
    void setEditedState(bool edited);
    void setFooter(QString footer) {this->footer = footer; setEditedState(true);}
    void setShowPageNumber(showPageNumberType showPageNumber) {this->showPageNumber = showPageNumber;}
    void setShowPageCount(bool showPageCount) {this->showPageCount = showPageCount;}
    int OpenFromIO(QIODevice &file);

signals:
    void EditedStateChanged(bool edited);
    void useMaskChanged(int useMask);
};

#endif // WORDSEARCHDOC_H
