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
#include <QString>
#include <QTextBlock>
#include <QTextCodec>
#include <QDesktopServices>

#include "listcreator.h"

listCreator::listCreator(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    connect(genlistButton, SIGNAL(clicked()), this, SLOT(generatelist()));
    connect(openButton, SIGNAL(clicked()), this, SLOT(fileOpen()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    connect(helpButton, SIGNAL(clicked()), this, SLOT(help()));
}

void listCreator::generatelist()
{
    QString pt = sourceTextEdit->document()->toPlainText();
    QStringList wl;
    int start = 0;
    int min = minSize->value(), max = 1000000, maxwords = 1000000;
    QString word;
    if (maxSize->value() > 0) max = maxSize->value();
    if (maxWords->value() > 0) maxwords = maxWords->value();
    for (int a = 0; a < pt.length() && wl.count() < maxwords; a++)
    {
        if (pt.at(a).isSpace() || a == pt.length()-1)
        {
            word = pt.mid(start, a - start).trimmed();
            if (word.endsWith(",") || word.endsWith("."))
                word = word.mid(0, word.length()-1);
            if ( word.length() >= min && word.length() <= max && wl.indexOf(word)==-1)
                wl << word;
            start=a;
        }
    }
    for (int a = 0; a < wl.count(); a++)
    {
        listTextEdit->insertPlainText(wl.at(a)+"\n");
    }
    this->wl = wl;
}

bool listCreator::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str)) {
        sourceTextEdit->setHtml(str);
    } else {
        str = QString::fromLocal8Bit(data);
        sourceTextEdit->setPlainText(str);
    }
    return true;
}

void listCreator::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              QString(), tr("HTML-Files (*.htm *.html);;Text-Files (*.txt);;All Files (*)"));
    if (!fn.isEmpty())
        load(fn);
}

void listCreator::okPressed()
{
    wl.clear();
    QTextCursor newCursor(listTextEdit->document());
    do{
        if (newCursor.block().text().trimmed()!="")
        {
            wl << newCursor.block().text().trimmed().toLocal8Bit().data();
        }
    }
    while(newCursor.movePosition(QTextCursor::Down));
    this->accept();
}

void listCreator::help()
{
    QDesktopServices::openUrl(QUrl("http:/wordsearchcreator.org/doc/1.2/listcreator"));
}
