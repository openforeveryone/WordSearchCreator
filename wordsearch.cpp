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

#include "wordsearch.h"

wordsearch::wordsearch()
{
	xsize=5;
	ysize=5;
	creategrid(xsize,ysize);
	resetgrid();
	resetmask();
	for (int a = 0; a<8 ; a++)
		setDirectionAllowed(a,false);
	setDirectionAllowed(2,true);
	setDirectionAllowed(4,true);
	setXSize(0);
	autoresize=false;
    wordSpace=FillSpace;
}

void wordsearch::creategrid(int x, int y)
{
	xsize = x;
	ysize = y;
	grid = new QChar * [x+2];
	mask = new bool * [x+2];
	for (int a = 0; a < x+2; a++)
	{
		grid[a] = new QChar [y+2];
		mask[a] = new bool [y+2];
	}
}

void wordsearch::deletegrid()
{
	int x = xsize;
	for (int a = 0; a < x+2; a++)
	{
		delete grid[a];
	}
	delete grid;
}

void wordsearch::dispgrid()
{
/*
	int y = ysize;
	int x = xsize;
	QChar letter;
	for (int b = 0; b < y+2; b++)
	{
		for (int a = 0; a < x+2; a++)
		{
			letter = grid [a][b];
			//std::cout << letter;
		}
		//std::cout << std::endl;
		
	}
*/
}

bool wordsearch::placeword(Word *temp, int x, int y, int direction)
{
	//check the word can be placed
	if (DirectionAllowed(direction)==false)
		return false;
	int posx, posy;
	posx = x;
	posy = y;
	bool nogap = true;
	//check if word fits
    for (int a = 0; a < temp->gridWord.size(); a++)
	{
		//cout<<direction<<x<<y<<grid[x][y]<<temp->word.at(a)<< (grid[x][y] == temp->word[a]) << (grid[x][y] == ' ') << endl;
        if (!((grid[x][y] == temp->gridWord[a]) or (grid[x][y] == QChar(' '))))
			return false; //word does not fit here
		if (grid[x][y] == QChar(' ')) nogap = false;
		switch (direction){
			case 2:
				x++;
				break;
			case 3:
				x++;
				y++;
				break;
			case 4:
				y++;
				break;
			case 5:
				x--;
				y++;
				break;
			case 6:
				x--;
				break;
			case 7:
				x--;
				y--;
				break;
			case 0:
				y--;
				break;
			case 1:
				x++;
				y--;
				break;
		}
	}
	//check that this word is not a compleatly inside another.
	if (nogap==true) return false;
	// set word's propities
	temp->used = true;
	temp->xpos = posx;
	temp->ypos = posy;
	temp->direction = direction;
	return true;
}

void wordsearch::placeingrid(Word *temp)
{
	int x = temp->xpos;
	int y = temp->ypos;
	int direction = temp->direction;
	// put word in grid
    for (int a = 0; a < temp->gridWord.size(); a++)
	{
        grid[x][y] = temp->gridWord.at(a);
        if (a != temp->gridWord.size()-1)
		{
		switch (direction){
			case 2:
				x++;
				break;
			case 3:
				x++;
				y++;
				break;
			case 4:
				y++;
				break;
			case 5:
				x--;
				y++;
				break;
			case 6:
				x--;
				break;
			case 7:
				x--;
				y--;
				break;
			case 0:
				y--;
				break;
			case 1:
				x++;
				y--;
				break;
		}
		}
	}
	temp->expos = x;
	temp->eypos = y;
}

void wordsearch::create()
{
	//ofstream file("debug.txt");
	//cout.rdbuf (file.rdbuf());
	stop=false;
	//cout << "sorting" << endl;
	sort(wordlist.begin(),wordlist.end());
	//random_shuffle(wordlist.begin(),wordlist.end());
	//declare the nesessary vairables
	uint i, wordsfitted, mostwordsfitted;

    //Remove spaces from words
    i = 0;
    while(i < wordlist.size())
    {
        wordlist[i].gridWord=wordlist[i].word;
        if (wordSpace==RemoveSpace)
            wordlist[i].gridWord.remove(' ');
        else if(wordSpace==KeepSpace)
            wordlist[i].gridWord.replace(' ', '\t');
        i++;
    }

	int x,y,d, attempts, sizeincs, attemptlimit;
	//stop the program wasting time if autoresize is enabled
	if (autoresize) {attemptlimit = 5;} else {attemptlimit = xsize * ysize * wordlist.size();}
	mostwordsfitted=0;
	vector<Word> tempwordlist, backupwordlist;
	tempwordlist = wordlist;
	backupwordlist = wordlist;
	sizeincs = 0;
	//size increace loop
	while (sizeincs < 100)
	{
	//cout << "Starting attempt loop" << endl;
	//attempt loop
	for (attempts = 0; (!stop) && attempts < attemptlimit; attempts++)
	{
		wordsfitted=0;
		//cout << "clearing grid" << endl;
		resetgrid();
		QApplication::processEvents();
		//make attempt
		i = 0;
		//cout << "making attempt: "<< attempts + 1 << endl;
		while ((!stop) && i < tempwordlist.size())
		{
			x = int((double(rand())/(RAND_MAX+1.0))*xsize) + 1;
			y = int((double(rand())/(RAND_MAX+1.0))*ysize) + 1;
			d = int((double(rand())/(RAND_MAX+1.0))*8);
			//cout << "Trying " << tempwordlist[i].word << endl;
			if (placeword(&tempwordlist[i], x,y,d))
			{
				//cout << "Placing " << tempwordlist[i].word << endl;
				placeingrid(&tempwordlist[i]);
				i++;
				wordsfitted++;
				QApplication::processEvents();
			}
			else
			{
				tempwordlist[i].tries++;
				if (tempwordlist[i].tries > xsize * ysize * 8)
				{
					i++;
				}
			}
			//cout << "Tries: " << tempwordlist[i].tries << endl;
		}
		if (wordsfitted > mostwordsfitted)
		{
			//cout << "best yet" << endl;
			wordlist=tempwordlist;
			mostwordsfitted=wordsfitted;
		}
		if (wordsfitted == tempwordlist.size())
		{
			//cout << "sucsessfull " << wordsfitted << " of " << tempwordlist.size() << " fitted. Attempts: " << attempts << endl;
			//sucsessfull
			wordlist=tempwordlist;
			addletters();
			return;
		}
		tempwordlist = backupwordlist;
	}
	if ((!autoresize) || stop)
	{
		break;
	}
	//cout << "resizing " << endl;
	setXSize(xsize+1);
	setYSize(ysize+1);
	sizeincs++;
	}
	resetgrid();
	for (i = 0; i < tempwordlist.size(); i++)
	{
		//cout << i << endl;
		if (wordlist[i].used == true)
			placeingrid(&wordlist[i]);
	}
	addletters();
	//cout << "Giving up at " << attempts << " attempts. fitted " << mostwordsfitted << " of " << tempwordlist.size() << endl;
	//file.close();
}

void wordsearch::addletters()
{
	for (int b = 0; b < ysize+2; b++)
	{
		for (int a = 0; a < xsize+2; a++)
		{
			if (grid [a][b] == QChar(' '))
			{
				if  (Alphabet.isNull())
					grid [a][b] = QChar(char(int((double(rand())/RAND_MAX)*26) + 97));
				else
					grid [a][b] = Alphabet.at(int((double(rand())/RAND_MAX)*Alphabet.size()));
					//cout << Alphabet.size();
			}
		}
	}
}

QChar wordsearch::Letter(int x, int y)
{
	if ((x < 0) or (y < 0))
		return QChar();
	if ((x > xsize) or (y > ysize))
		return QChar();
	x++;
	y++;
	return grid [x][y];
}

void wordsearch::placeLetter(QChar letter, int x, int y)
{
	if ((x < 0) or (y < 0))
		return;
	if ((x > xsize) or (y > ysize))
		return;
	x++;
	y++;
	grid [x][y]=letter;
}

void wordsearch::allowLetter(bool allow, int x, int y)
{
	if ((x < 0) or (y < 0))
		return;
	if ((x > xsize-1) or (y > ysize-1))
		return;
	x++;
	y++;
	mask [x][y]=allow;
}

void wordsearch::setXSize (int x)
{
	deletegrid();
	xsize = x;
	creategrid(xsize, ysize);
	resetmask();
	resetgrid();
}

void wordsearch::setYSize (int y)
{
	deletegrid();
	ysize = y;
	creategrid(xsize, ysize);
	resetmask();
	resetgrid();
}

void wordsearch::setAutoResize (bool autoresize)
{
	this->autoresize=autoresize;
}

bool wordsearch::AutoResize ()
{
	return autoresize;
}

void wordsearch::resetgrid()
{
	if (autoresize) resetmask();
	int x = xsize;
	int y = ysize;
	for (int a = 0; a < x+2; a++)
	{
		for (int b = 0; b < y+2; b++)
		{
			if (mask [a][b] == true) {grid [a][b] = QChar(' ');} else {grid [a][b] = QChar();}
		}
	}
}

void wordsearch::resetmask()
{
	//cout << "resetting grid" << endl;
	int x = xsize;
	int y = ysize;
	for (int a = 0; a < x+2; a++)
	{
		for (int b = 0; b < y+2; b++)
		{
			mask [a][b] = true;
		}
	}
	for (int a = 0; a < x+2; a++)
	{
		mask[a][0] = false;
		mask[a][y+1] = false;
	}
	for (int b = 1; b < y+2; b++)
	{
		mask [0][b] = false;
		mask [x+1][b] = false;
	}
}
bool Word::operator==(Word tempword)
{
	if (this->word!=tempword.word) return false;
	if (this->xpos!=tempword.xpos) return false;
	if (this->ypos!=tempword.ypos) return false;
	if (this->used!=tempword.used) return false;
	if (this->direction!=tempword.direction) return false;
	return true;
}

bool Word::operator<(const Word tempword) const
{
	return (tempword.word.size() < this->word.size());
}

Word::Word()
{
	word="";
	xpos=0;
	ypos=0;
	direction=0;
	tries=0;
	used=false;
	answered=false;
}

Word::Word(QString word)
{
	this->word=word;
	xpos=0;
	ypos=0;
	direction=0;
	tries=0;
	used=false;
}
