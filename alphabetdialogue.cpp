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

#include <QInputDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QProgressDialog>
#include <QtXml>
#include <iostream>

#include "wordsearchdoc.h"

#include "alphabetdialogue.h"

AlphabetDialog::AlphabetDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    this->setWindowModality(Qt::WindowModal);
    //http= new QHttp(this);
    networkAccessManager = new QNetworkAccessManager();
    Alphabetcbox->addItem("Custom");
    Alphabetcbox->addItem("English - \"abcdefghijklmnopqrstuvwxyz\"","abcdefghijklmnopqrstuvwxyz");
    Alphabetcbox->addItem("Numbers - \"1234567890\"","1234567890");
    QSettings settings;
    settings.beginGroup("alphabets");
    QStringList keys = settings.allKeys();
    for (int i = 0; i < keys.size(); ++i)
    {
        QString alphabetname = keys.at(i);
        QString alphabet = settings.value(alphabetname).toString();
        alphabetname.append(" - \"").append(alphabet).append("\"");
        Alphabetcbox->addItem(alphabetname,alphabet);
    }
    connect(Alphabetcbox, SIGNAL(currentIndexChanged(int)), this, SLOT(cboxchanged(int)));
    connect(MakeUpper, SIGNAL(clicked()), this, SLOT(toUpper()));
    connect(MakeLower, SIGNAL(clicked()), this, SLOT(toLower()));
    connect(AlphabetBox, SIGNAL(textEdited(QString)), this, SLOT(abedeted(QString)));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(GetFromWeb, SIGNAL(clicked()), this, SLOT(getfromweb()));
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpRequestFinished(QNetworkReply*)));
}

void AlphabetDialog::okPressed()
{
    if (Alphabetcbox->currentIndex()!=1)
        wsd->ws->Alphabet=AlphabetBox->text();
    else
        wsd->ws->Alphabet=QString();
    accept();
}

void AlphabetDialog::toUpper()
{
    AlphabetBox->setText(AlphabetBox->text().toUpper());
    abedeted(AlphabetBox->text());
}

void AlphabetDialog::toLower()
{
    AlphabetBox->setText(AlphabetBox->text().toLower());
    abedeted(AlphabetBox->text());
}

void AlphabetDialog::setDoc(WordSearchDoc *wsd)
{
    this->wsd = wsd;
    AlphabetBox->setText(wsd->ws->Alphabet);
    if (wsd->ws->Alphabet.isNull())
    {
        Alphabetcbox->setCurrentIndex(1);
        saveButton->setEnabled(false);
    }
    else
        abedeted(AlphabetBox->text());
}

void AlphabetDialog::cboxchanged(int index)
{
    QString text = Alphabetcbox->itemData(index).toString();
    if (!text.isNull())
        AlphabetBox->setText(text);

}

void AlphabetDialog::abedeted(QString text)
{
    bool found = false;
    for (int a = 1; a < Alphabetcbox->count() && found == false; a++)
    {
        //        std::cout << a << std::endl;
        QString text1=Alphabetcbox->itemData(a).toString();
        found = true;
        if (text.count() != text1.count())
            found = false;
        if (found)
        {
            for (int b = 0; b < text.count(); b++)
            {
                if (!text1.contains(text.at(b)))
                {
                    //                    std::cout << "yes" << std::endl;
                    found = false;
                    break;
                }
            }
        }
        if (found)
        {
            for (int b = 0; b < text1.count(); b++)
            {
                if (!text.contains(text1.at(b)))
                {
                    found = false;
                    break;
                }
            }
        }
        if (found)
        {
            Alphabetcbox->setCurrentIndex(a);
            saveButton->setEnabled(false);
        }
    }
    if (!found)
    {
        Alphabetcbox->setCurrentIndex(0);
        saveButton->setEnabled(true);
    }

}

void AlphabetDialog::save()
{
    QSettings settings;
    settings.beginGroup("alphabets");
    bool ok;
    QString alphabetname = QInputDialog::getText(this, tr("Enter name of alphabet:"),
                                                 tr("Enter name of alphabet:"), QLineEdit::Normal,
                                                 "", &ok);
    if (ok && !alphabetname.isEmpty())
    {
        settings.setValue(alphabetname, AlphabetBox->text());
        alphabetname.append(" - \"").append(AlphabetBox->text()).append("\"");
        Alphabetcbox->addItem(alphabetname,AlphabetBox->text());
        abedeted(AlphabetBox->text());
    }
}

void AlphabetDialog::getfromweb()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://wordsearchcreator.org"));
    networkAccessManager->get(QNetworkRequest(QUrl("http://wordsearchcreator.org/alphabets.xml")));
}

void AlphabetDialog::httpRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError){
        
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed: %1.")
                                 .arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    QDomDocument doc;
    if (doc.setContent(reply))
    {
        QDomElement root = doc.documentElement();
        if( root.tagName() != "WSCXMLal" )
        {
            QMessageBox::information(this, tr("Wordsearch Creator"), tr("Incorrect file format."));
            reply->deleteLater();
            return;
        }
        QSettings settings;
        settings.beginGroup("alphabets");
        QDomNodeList DEWordList = root.elementsByTagName("alphabet");

        for (int i = 0; i < DEWordList.count(); i++)
        {
            QString alphabet = DEWordList.at(i).toElement().text();
            QString alphabetname = DEWordList.at(i).toElement().attribute("name");
            QString ealphabet = settings.value(alphabetname,QString()).toString();
            if (!alphabetname.isEmpty() && !alphabet.isEmpty() && ealphabet.isNull())
            {
                settings.setValue(alphabetname, alphabet);
                alphabetname.append(" - \"").append(alphabet).append("\"");
                Alphabetcbox->addItem(alphabetname,alphabet);
            }
        }
        QMessageBox::information(this, tr("Wordsearch Creator"),
                                 tr("Alphabet list updated."));
    }
    else
        QMessageBox::information(this, tr("Wordsearch Creator"),
                                 tr("XML Error."));
    reply->deleteLater();
    std::cout << "ok" << std::endl;
}
