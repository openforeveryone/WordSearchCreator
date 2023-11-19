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

#ifndef WORDSEARCH_H
#define WORDSEARCH_H

#include <QApplication>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

class Word
{
public:
    Word();
    Word(QString word);
    QString word;
    QString gridWord;
    void setword();
    bool used;
    int xpos, ypos, expos, eypos, listpos;
    int direction;
    int tries;
    bool answered;
    bool operator== (Word tempword);
    bool operator< (const Word tempword) const;
};

class wordsearch
{
private:
    void creategrid(int x, int y);
    QChar **grid;
    bool **mask;
    int xsize;
    int ysize;
    bool placeword(Word *temp, int x, int y, int direction);
    void placeingrid(Word *temp);
    void addletters();
    bool alloweddirection[8];
    bool autoresize;
    void deletegrid();

public:
    void resetgrid();
    bool stop;
    void dispgrid();
    wordsearch();
    void resetmask();//fcre
    QChar Letter(int x, int y);
    bool Mask(int x, int y) { return mask[x][y]; }
    void placeLetter(QChar letter, int x, int y);
    void allowLetter(bool allow, int x, int y);
    int XSize() { return xsize; }
    int YSize() { return ysize; }
    void setXSize (int x);
    void setYSize (int y);
    vector<Word> wordlist;
    void setDirectionAllowed (int direction, bool Allowed)
        {alloweddirection[direction]=Allowed;}
    bool DirectionAllowed (int direction)
        {return alloweddirection[direction];}
    void create();
    void setAutoResize(bool autoresize);
    bool AutoResize();
    bool UseMask();
    QString Alphabet;
    enum wordSpaceType {FillSpace, RemoveSpace, KeepSpace};
    wordSpaceType wordSpace;
};

#endif
