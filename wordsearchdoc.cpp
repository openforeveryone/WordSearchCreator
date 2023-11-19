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


#include <QWidget>
#include <QFile>
#include <QDomDocument>
#include <QDebug>

#include "wordsearchdoc.h"

WordSearchDoc::WordSearchDoc(QWidget *parent)
    : QObject(parent)
{
    WSFont.setFamily("URW Gothic L");
    WSFont.setPointSize(20);
    WSFont.setWeight(QFont::Light);
    creatorLabelFont.setPointSize(8);
    ws = new wordsearch;
    showanswers=false;
    showwords=false;
    showCreatorLabel=true;
    showGrid=true;
    useMask=false;
    maskChanged=false;
    edited=false;
    sortmethod = 2;
    gridWidth=0.02;
    topMargin = 0.5; leftMargin = 0.5; rightMargin = 0.5; bottomMargin = 0.5; pageheight = 11.69; pagewidth = 8.26;
    filename = "";
    wordlistorder=UserDefined;
    bgColor=Qt::white;
    hlColor=Qt::blue;
}

void WordSearchDoc::Create()
{
    ws->create();
    setEditedState(true);

    //        update();
}

void WordSearchDoc::setWSFont(QFont WSFont)
{
    this->WSFont = WSFont;
    setEditedState(true);
    //        update();
}

uint WordSearchDoc::numwordsplaced()
{
    int numwordsplaced = 0;
    for (uint i = 0; i < ws->wordlist.size(); i++)
    {
        if (ws->wordlist[i].used)
            numwordsplaced++;
    }
    return numwordsplaced;
}

void WordSearchDoc::setWSFont()
{
    //        bool ok;
    //        QFont font = QFontDialog::getFont(&ok, this->WSFont, this);
    //        if (ok) {
    //                this->WSFont = font;
    //                update();
    //        }
}

void WordSearchDoc::setTitleFont(QFont TitleFont)
{
    this->TitleFont = TitleFont;
    setEditedState(true);
    //update();
}

void WordSearchDoc::setTitleFont()
{
    //        bool ok;
    //        QFont font = QFontDialog::getFont(&ok, this->TitleFont, this);
    //        if (ok)
    //        {
    //                this->TitleFont = font;
    ////                update();
    //        }
}

void WordSearchDoc::setWordsFont(QFont WordsFont)
{
    this->WordsFont = WordsFont;
    setEditedState(true);
    //        update();
}

void WordSearchDoc::setWordsFont()
{
    //        bool ok;
    //        QFont font = QFontDialog::getFont(&ok, this->WordsFont, this);
    //        if (ok) {
    //                this->WordsFont = font;
    //                update();
    //        }
}

QFont WordSearchDoc::getWordsFont()
{
    return WordsFont;
}

QFont WordSearchDoc::getWSFont()
{
    return WSFont;
}

QFont WordSearchDoc::getTitleFont()
{
    return TitleFont;
}

QFont WordSearchDoc::getCLFont()
{
    return creatorLabelFont;
}

void WordSearchDoc::setSize(int x, int y)
{
    ws->setXSize(x);
    ws->setYSize(y);
    setEditedState(true);
    //update();
}

void WordSearchDoc::setXSize(int x)
{
    if (x!=ws->XSize())
    {
        ws->setXSize(x);
        ws->wordlist.clear();
        setEditedState(true);
        //update();
    }
}

void WordSearchDoc::setYSize(int y)
{
    if (y!=ws->YSize())
    {
        ws->setYSize(y);
        ws->wordlist.clear();
        setEditedState(true);
        //update();
    }
}

void WordSearchDoc::setShowAnswers(int showanswers)
{
    this->showanswers=showanswers;
    setEditedState(true);
    //update();
}

void WordSearchDoc::setTitle(QString Title)
{
    title=Title;
    setEditedState(true);
    //update();
}

bool WordSearchDoc::getShowWords()
{
    return showwords;
}

bool WordSearchDoc::getShowAnswers()
{
    return showanswers;
}

QString WordSearchDoc::getTitle()
{
    return title;
}

void WordSearchDoc::setShowWords(int showwords)
{
    this->showwords=showwords;
    setEditedState(true);
    //update();
}

void WordSearchDoc::setEditedState(bool edited)
{
    this->edited=edited;
    emit EditedStateChanged(edited);
}

void WordSearchDoc::setAutoResize(int autosize)
{
    
    if (autosize>0)
        ws->setAutoResize(true);
    else
        ws->setAutoResize(false);
    setEditedState(true);
}

int WordSearchDoc::Open(QString fileName)
{
    
    if (fileName.isEmpty())
        return 2;
    QFile file(fileName);
    
    if (!file.open(QIODevice::ReadOnly))
        return 2;
    int error = OpenFromIO(file);
    if (!error)
    {
        this->filename=fileName;
        //        qDebug() << "File opened" << filename;
        setEditedState(false);
    }
    return error;
}

int WordSearchDoc::OpenFromIO(QIODevice &file)
{
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        //        file.close();
        //        if (!file.open(QIODevice::ReadOnly))
        //            return 2;
        file.seek(0);
        qDebug() << "File is binary";
        int numwords;
        QDataStream in(&file);
        in.setVersion(8);
        QString filetype;
        in >> filetype;
        int fileversion;
        if (filetype == "Wordsearch Creator wordsearch")
            fileversion = 1;
        else if (filetype == "Wordsearch Creator wordsearch 0.9")
            fileversion = 2;
        else
            return 3;
        int y = ws->YSize();
        int x = ws->XSize();
        in >> x >> y;
        setYSize(y);
        setXSize(x);
        for (int b = 0; b < y; b++)
        {
            for (int a = 0; a < x; a++)
            {
                QChar tempQChar;
                in >> tempQChar;
                ws->placeLetter(tempQChar, a, b);
            }
        }
        QFont WSFont, TitleFont, WordsFont;
        bool showanswers, showwords, useMask, showGrid;
        double gridWidth;
        QColor WSColor, titleColor, WLColor, gridColor;
        QString title;
        in >> WSFont >> TitleFont >> WordsFont >> showanswers >> showwords >> title >> useMask >> showGrid >> WSColor >> titleColor >> WLColor >> gridColor >> gridWidth;
        setShowGrid(showGrid);
        setWSColor(WSColor);
        setTitleColor(titleColor);
        setWLColor(WLColor);
        setGridColor(gridColor);
        setGridWidth(gridWidth);
        setWSFont(WSFont);
        setWordsFont(WordsFont);
        setTitleFont(TitleFont);
        setMaskChanged(false);
        setUseMask(useMask);
        if (useMask)
            setMaskChanged(true);
        this->setShowAnswers(showanswers);
        this->setShowWords(showwords);
        setTitle(title);

        ws->wordlist.clear();
        in >> numwords;

        for (int a=0; a < numwords; a++)
        {
            QString worda;
            Word temp;
            in >> worda;
            temp.word = worda;
            ws->wordlist.push_back(temp);
            in >> ws->wordlist[a].used >> ws->wordlist[a].xpos >> ws->wordlist[a].ypos >> ws->wordlist[a].expos >> ws->wordlist[a].eypos >> ws->wordlist[a].direction;
            if (fileversion == 2)
                in >> ws->wordlist[a].listpos;
            else
                ws->wordlist[a].listpos = a;
            ws->wordlist[a].word = worda;


        }
        for (int b = 0; b < y; b++)
        {
            for (int a = 0; a < x; a++)
            {
                bool mc;
                in >> mc;
                ws->allowLetter(mc, a, b);
            }
        }
        //fill sortedwordlist in "UserDefined" order for wsc to populate the textbox
        //        int a=0;
        //        this->sortedwordlist.clear();
        //        while (a < this->ws->wordlist.size())
        //        {
        //            this->sortedwordlist.append(this->ws->wordlist.at(a));
        //            a++;
        //        }
        //        a=0;
        //        while (a < this->sortedwordlist.size()-1)
        //        {
        //            if ( this->sortedwordlist.at(a).listpos > this->sortedwordlist.at(a+1).listpos)
        //            {
        //                this->sortedwordlist.move(a+1, a);
        //                a=0;
        //            }
        //            else
        //                a++;
        //        }
        file.close();
    }
    else
    {
        file.close();
        QDomElement root = doc.documentElement();
        if( root.tagName() != "WSCXML" )
        {
            return 3;
        }
        QDomElement worksheet = root.firstChild().toElement();
        if(worksheet.tagName() != "Worksheet") return 3;
        
        QDomElement wordsearch = worksheet.firstChildElement("Wordsearch");
        if(wordsearch.tagName() != "Wordsearch") return 3;
        
        
        int x = wordsearch.attribute( "xsize","-1").toInt();
        int y = wordsearch.attribute( "ysize","-1").toInt();
        if (x<1 || x>99 || y<1 || y>99) return 3;
        
        
        setYSize(y);
        setXSize(x);
        ws->resetmask();
        ws->resetgrid();
        
        QDomElement DEtitle = wordsearch.firstChildElement("Title");
        setTitle(DEtitle.text());
        
        ws->Alphabet=wordsearch.firstChildElement("Alphabet").text();
        
        this->setShowAnswers((wordsearch.attribute("ShowAnswers","no").toLower() == "yes" || wordsearch.attribute("ShowAnswers","no").toLower() =="true"));
        this->setShowWords((wordsearch.attribute("ShowWords","no").toLower() == "yes" || wordsearch.attribute("ShowWords","no").toLower() =="true"));

        QDomElement DEAllowedDirections = wordsearch.firstChildElement("AllowedDirections");
        if (!DEAllowedDirections.isNull())
        {
            for (int x = 0; x < 8; x++)
            {
                if (DEAllowedDirections.attribute(QString("Direction%1").arg(x)).toLower() == "yes")
                    ws->setDirectionAllowed(x,true);
                else
                    ws->setDirectionAllowed(x,false);
            }
        }
        QDomElement DEFormat = wordsearch.firstChildElement("Format");
        QFont WSFont;
        QColor WSColor;
        WSFont.fromString(DEFormat.firstChildElement("WSFont").attribute("qtFontDescription"));
        WSColor.setNamedColor(DEFormat.firstChildElement("WSFont").attribute("color"));
        setWSFont(WSFont);
        setWSColor(WSColor);
        QFont TitleFont;
        QColor TitleColor;
        TitleFont.fromString(DEFormat.firstChildElement("TitleFont").attribute("qtFontDescription"));
        TitleColor.setNamedColor(DEFormat.firstChildElement("TitleFont").attribute("color"));
        setTitleFont(TitleFont);
        setTitleColor(TitleColor);
        QFont WordsFont;
        QColor WordsColor;
        WordsFont.fromString(DEFormat.firstChildElement("WordsFont").attribute("qtFontDescription"));
        WordsColor.setNamedColor(DEFormat.firstChildElement("WordsFont").attribute("color"));
        setWordsFont(WordsFont);
        setWordsColor(WordsColor);
        QColor BGColor;
        BGColor.setNamedColor(DEFormat.firstChildElement("Background").attribute("color", "white"));
        setBgColor(BGColor);
        QColor HLColor;
        HLColor.setNamedColor(DEFormat.firstChildElement("Highlight").attribute("color", "blue"));
        setHlColor(HLColor);
        setShowGrid((DEFormat.firstChildElement("Grid").attribute("Visible","yes").toLower() == "yes" || wordsearch.attribute("Visible","yes").toLower() =="true"));
        QColor GridColor;
        GridColor.setNamedColor(DEFormat.firstChildElement("Grid").attribute("color","black"));
        setGridColor(GridColor);
        setGridWidth(DEFormat.firstChildElement("Grid").attribute("Width","0").toDouble());
        
        ws->resetgrid();
        QDomElement DELetters = wordsearch.firstChildElement("Letters");
        QDomNodeList DELetterList = DELetters.elementsByTagName("Letter");
        for (int i = 0; i < DELetterList.count(); i++)
        {
            int a=DELetterList.at(i).toElement().attribute( "XPos").toInt();
            int b=DELetterList.at(i).toElement().attribute( "YPos").toInt();
            //A check should be done here
            if (DELetterList.at(i).toElement().text().size() > 0)
                ws->placeLetter(DELetterList.at(i).toElement().text().at(0), a, b);
        }
        
        ws->wordlist.clear();

        QStringList orders;
        orders.append("UserDefined");
        orders.append("Alphabetical");
        orders.append( "ReverseAlphabetical");
        orders.append( "SizeAsc");
        orders.append( "SizeDes");
        QStringList wordSpaceLabels;
        wordSpaceLabels.append("FillSpace");
        wordSpaceLabels.append("RemoveSpace");
        wordSpaceLabels.append("KeepSpace");
        QDomElement DEWords = wordsearch.firstChildElement("Words");
        if (DEWords.attributes().contains("Order"))
        {

        }
        if (DEWords.attributes().contains("SpaceBehaviour"))
        {

        }

        QDomNodeList DEWordList = DEWords.elementsByTagName("Word");
        
        for (int i = 0; i < DEWordList.count(); i++)
        {
            //cout <<i<<endl;
            Word temp;
            temp.word = DEWordList.at(i).toElement().text();
            temp.listpos = DEWordList.at(i).toElement().attribute( "ListPos").toInt();
            if (DEWordList.at(i).toElement().attribute("Used").toLower()=="yes")
            {
                temp.used = true;
                temp.xpos = DEWordList.at(i).toElement().attribute( "StartXPos").toInt();
                temp.ypos = DEWordList.at(i).toElement().attribute( "StartYPos").toInt();
                temp.expos = DEWordList.at(i).toElement().attribute( "EndXPos").toInt();
                temp.eypos = DEWordList.at(i).toElement().attribute( "EndYPos").toInt();
                temp.direction = DEWordList.at(i).toElement().attribute( "Direction").toInt();
                if (DEWordList.at(i).toElement().attribute("Answered","false")=="yes"||DEWordList.at(i).toElement().attribute("Answered","false")=="true")
                    temp.answered = true;
                else
                    temp.answered = false;
            }
            //cout << temp.word << " " << temp.xpos << " " << temp.ypos << " " << temp.expos  << " " << temp.eypos  << " " << temp.direction << " " << endl;
            ws->wordlist.push_back(temp);
        }
        
        QDomElement DEMask = wordsearch.firstChildElement("Mask");
        
        ws->resetmask();
        setMaskChanged(false);
        setUseMask(false);
        
        if (!DEMask.isNull())
        {
            setMaskChanged(true);
            QDomNodeList DECellList = DEMask.elementsByTagName("Cell");
            for (int i = 0; i < DECellList.count(); i++)
            {
                //cout << DECellList.at(i).toElement().attribute("XPos").toInt() << " " << DECellList.at(i).toElement().attribute("YPos").toInt() << cout;
                ws->allowLetter(false, DECellList.at(i).toElement().attribute("XPos").toInt(), DECellList.at(i).toElement().attribute("YPos").toInt());
                setUseMask(true);
            }
        }
        
        QDomElement pagelayout = worksheet.firstChildElement("PageLayout");
        
        leftMargin = pagelayout.attribute( "LeftMargin", "0.5").toDouble();
        rightMargin = pagelayout.attribute( "RightMargin", "0.5").toDouble();
        topMargin = pagelayout.attribute( "TopMargin", "0.5").toDouble();
        bottomMargin = pagelayout.attribute( "BottomMargin", "0.5").toDouble();
        pageheight = pagelayout.attribute( "Height", "11.69").toDouble();
        pagewidth = pagelayout.attribute( "Width", "8.26").toDouble();
        
        QDomElement DEfooter = worksheet.firstChildElement("Footer");
        footer = DEfooter.text();
        
        //fill sortedwordlist in "UserDefined" order for wsc to populate the textbox
        //        int a=0;
        //        this->sortedwordlist.clear();
        //        while (a < this->ws->wordlist.size())
        //        {
        //            this->sortedwordlist.append(this->ws->wordlist.at(a));
        //            a++;
        //        }
        //        a=0;
        //        while (a < this->sortedwordlist.size()-1)
        //        {
        //            if ( this->sortedwordlist.at(a).listpos > this->sortedwordlist.at(a+1).listpos)
        //            {
        //                this->sortedwordlist.move(a+1, a);
        //                a=0;
        //            }
        //            else
        //                a++;
        //        }

    }
    setEditedState(false);
    return 0;
}

bool WordSearchDoc::saveToIO(QIODevice &file){
    QTextStream out(&file);
    out.setCodec("UTF-8");
    QDomDocument doc;
    QDomElement root = doc.createElement("WSCXML");
    doc.appendChild(root);
    QDomElement worksheet = doc.createElement("Worksheet");
    root.appendChild(worksheet);
    
    QDomElement pagelayout = doc.createElement("PageLayout");
    worksheet.appendChild(pagelayout);
    
    pagelayout.setAttribute( "LeftMargin", leftMargin);
    pagelayout.setAttribute( "RightMargin", rightMargin);
    pagelayout.setAttribute( "TopMargin", topMargin);
    pagelayout.setAttribute( "BottomMargin",bottomMargin);
    pagelayout.setAttribute( "Height", pageheight);
    pagelayout.setAttribute( "Width", pagewidth);
    
    QDomElement wordsearch = doc.createElement("Wordsearch");
    worksheet.appendChild(wordsearch);
    wordsearch.setAttribute( "xsize", ws->XSize());
    wordsearch.setAttribute( "ysize", ws->YSize());
    
    if (getTitle() != "") {
        QDomElement DEtitle = doc.createElement("Title");
        wordsearch.appendChild(DEtitle);
        QDomText DTtitle = doc.createTextNode(getTitle());
        DEtitle.appendChild(DTtitle);}
    
    if (!ws->Alphabet.isNull())
    {
        QDomElement DEAlphabet = doc.createElement("Alphabet");
        wordsearch.appendChild(DEAlphabet);
        QDomText DTAlphabet = doc.createTextNode(ws->Alphabet);
        DEAlphabet.appendChild(DTAlphabet);
    }
    
    if(getShowAnswers())
        wordsearch.setAttribute ( "ShowAnswers","yes");
    
    if(getShowWords())
        wordsearch.setAttribute ( "ShowWords","yes");
    
    
    
    QDomElement DEFormat = doc.createElement("Format");
    wordsearch.appendChild(DEFormat);
    
    QDomElement DEGrid = doc.createElement("Grid");
    DEFormat.appendChild(DEGrid);
    if(!getShowGrid())
        DEGrid.setAttribute ("Visible","no");
    DEGrid.setAttribute ( "Width",getGridWidth());
    DEGrid.setAttribute( "color", getGridColor().name());
    
    QDomElement DEWSFont = doc.createElement("WSFont");
    DEFormat.appendChild(DEWSFont);
    DEWSFont.setAttribute( "qtFontDescription", getWSFont().toString());
    DEWSFont.setAttribute( "color", getWSColor().name());
    
    QDomElement DETitleFont = doc.createElement("TitleFont");
    DEFormat.appendChild(DETitleFont);
    DETitleFont.setAttribute( "qtFontDescription", getTitleFont().toString());
    DETitleFont.setAttribute( "color", getTitleColor().name());
    
    QDomElement DEWordsFont = doc.createElement("WordsFont");
    DEFormat.appendChild(DEWordsFont);
    DEWordsFont.setAttribute( "qtFontDescription", getWordsFont().toString());
    DEWordsFont.setAttribute( "color", getWLColor().name());

    QDomElement DEBackground = doc.createElement("Background");
    DEFormat.appendChild(DEBackground);
    DEBackground.setAttribute( "color", getBGColor().name());

    QDomElement DEHighlight = doc.createElement("Highlight");
    DEFormat.appendChild(DEHighlight);
    DEHighlight.setAttribute( "color", getHLColor().name());

    QDomElement DEAllowedDirections = doc.createElement("AllowedDirections");
    wordsearch.appendChild(DEAllowedDirections);
    for (int x = 0; x < 8; x++)
    {
        DEAllowedDirections.setAttribute(QString("Direction%1").arg(x),ws->DirectionAllowed(x)?"Yes":"No");
    }

    QDomElement DELetters= doc.createElement("Letters");
    wordsearch.appendChild(DELetters);
    
    int y = ws->YSize();
    int x = ws->XSize();
    QDomElement DELetter[x*y];
    QDomText DTLetter[x*y];
    for (int b = 0; b < y; b++)
    {
        for (int a = 0; a < x; a++)
        {
            if (!ws->Letter(a,b).isNull())
            {
                DELetter[(a*b)+a] = doc.createElement("Letter");
                DELetters.appendChild(DELetter[(a*b)+a]);
                DELetter[(a*b)+a].setAttribute( "XPos",a);
                DELetter[(a*b)+a].setAttribute( "YPos",b);
                DTLetter[(a*b)+a] = doc.createTextNode(ws->Letter(a,b));
                //cout << a << " " << b << " " << ws->Letter(a,b).toAscii () << endl;
                DELetter[(a*b)+a].appendChild(DTLetter[(a*b)+a]);
            }
        }
    }
    
    QDomElement DEWords = doc.createElement("Words");
    wordsearch.appendChild(DEWords);
    QStringList orders;
    orders.append("UserDefined");
    orders.append("Alphabetical");
    orders.append( "ReverseAlphabetical");
    orders.append( "SizeAsc");
    orders.append( "SizeDes");
    QStringList wordSpaceLabels;
    wordSpaceLabels.append("FillSpace");
    wordSpaceLabels.append("RemoveSpace");
    wordSpaceLabels.append("KeepSpace");
    DEWords.setAttribute("Order", orders.at(wordlistorder));
    DEWords.setAttribute("SpaceBehaviour", wordSpaceLabels.at(ws->wordSpace));
    QDomElement DEWord[ws->wordlist.size()];
    QDomText DTWord[ws->wordlist.size()];
    for (uint a=0; a < ws->wordlist.size(); a++)
    {
        DEWord[a] = doc.createElement("Word");
        DEWords.appendChild(DEWord[a]);
        if (ws->wordlist[a].used)
        {	DEWord[a].setAttribute("Used", "yes");
            DEWord[a].setAttribute( "StartXPos",ws->wordlist[a].xpos);
            DEWord[a].setAttribute( "StartYPos",ws->wordlist[a].ypos);
            DEWord[a].setAttribute( "EndXPos",ws->wordlist[a].expos);
            DEWord[a].setAttribute( "EndYPos",ws->wordlist[a].eypos);
            DEWord[a].setAttribute( "Direction",ws->wordlist[a].direction);
            if (ws->wordlist[a].answered)
                DEWord[a].setAttribute("Answered","yes");
        }
        DEWord[a].setAttribute( "ListPos",ws->wordlist[a].listpos);
        DTWord[a]= doc.createTextNode(ws->wordlist[a].word);
        DEWord[a].appendChild(DTWord[a]);
    }
    if (true)//getUseMask())
    {
        QDomElement DEMask = doc.createElement("Mask");
        wordsearch.appendChild(DEMask);
        QDomElement DEMaskCell[x*y];
        for (int b = 0; b < y; b++)
        {
            for (int a = 0; a < x; a++)
            {
                if (!ws->Mask(a+1,b+1))
                {
                    DEMaskCell[(a*b)+a] = doc.createElement("Cell");
                    DEMaskCell[(a*b)+a].setAttribute( "XPos",a);
                    DEMaskCell[(a*b)+a].setAttribute( "YPos",b);
                    DEMask.appendChild(DEMaskCell[(a*b)+a]);
                }
            }
        }
    }

    if (!this->footer.isNull())
    {
        QDomElement DEfooter = doc.createElement("Footer");
        worksheet.appendChild(DEfooter);
        QDomText DTfooter = doc.createTextNode(this->footer);
        DEfooter.appendChild(DTfooter);
    }
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << doc.toString();
    //qDebug() << doc.toString();
    return true;
}

QList<Word> WordSearchDoc::sortedWordList()
{
    int a = 0;
    QList<Word> sortedwordlist;
    while (a < int(ws->wordlist.size()))
    {
        sortedwordlist.append(ws->wordlist.at(a));
        a++;
    }
    a=0;
    if (wordlistorder==WordSearchDoc::Alphabetical)
        while (a < sortedwordlist.size()-1)
        {
            if ( QString::localeAwareCompare(sortedwordlist.at(a).word,sortedwordlist.at(a+1).word)>0)
            {
                sortedwordlist.move(a+1, a);
                a=0;
            }
            else
                a++;
        }
    if (wordlistorder==WordSearchDoc::UserDefined)
        while (a < sortedwordlist.size()-1)
        {
            if ( sortedwordlist.at(a).listpos > sortedwordlist.at(a+1).listpos)
            {
                sortedwordlist.move(a+1, a);
                a=0;
            }
            else
                a++;
        }
    if (wordlistorder==WordSearchDoc::ReverseAlphabetical)
        while (a < sortedwordlist.size()-1)
        {
            if ( QString::localeAwareCompare(sortedwordlist.at(a).word,sortedwordlist.at(a+1).word)<0)
            {
                sortedwordlist.move(a+1, a);
                a=0;
            }
            else
                a++;
        }
    if (wordlistorder==WordSearchDoc::SizeDes)
        while (a < sortedwordlist.size()-1)
        {
            if (sortedwordlist.at(a).word.size()>sortedwordlist.at(a+1).word.size())
            {
                sortedwordlist.move(a+1, a);
                a=0;
            }
            else
                a++;
        }
    return sortedwordlist;
}


void WordSearchDoc::clearAnswers()
{
    for(uint i = 0; i < ws->wordlist.size(); i++)
    {
        ws->wordlist[i].answered=false;
    }
    EditedStateChanged(true);
}

WordSearchDoc::~WordSearchDoc()
{
    delete ws;
}
