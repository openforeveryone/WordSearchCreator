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

#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QSettings>
#include <QDesktopServices>

#include "updatechecker.h"

UpdateChecker::UpdateChecker(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(updateReplyFinished(QNetworkReply*)));
}

void UpdateChecker::checkForUpdate(bool manual = false)
{
    QUrlQuery query;
    QUrl url(QString("http://www.wordsearchcreator.org/update/%1/check.php").arg(getVersion()));
    if (manual)
        query.addQueryItem("type", "manual");
    else
    {
        QSettings settings;
        bool autoCheckEnabled = settings.value("updateChecker/autoUpdateCheckEnabled", true).toBool();
        if (!autoCheckEnabled)
            return;
    }
#ifdef Q_OS_LINUX
    query.addQueryItem("OS", "Linux");
#elif defined Q_OS_MAC
    query.addQueryItem("OS", "Mac");
#elif defined Q_OS_WIN32
    query.addQueryItem("OS", "Windows");
#else
    query.addQueryItem("OS", "Unknown");
#endif
    url.setQuery(query);
    manager->get(QNetworkRequest(url));
}

void UpdateChecker::updateReplyFinished(QNetworkReply* reply)
{
    QUrlQuery query(reply->request().url());
    bool manualRequest = query.queryItemValue("type")=="manual";
    //Check that there was no error.
    if (reply->error()!=QNetworkReply::NoError)
    {
        if (manualRequest) //If the check was manual and there was an error then show the browser.
            QDesktopServices::openUrl(QUrl(QString("http://wordsearchcreator.org/update/").append(getVersion())));
        //If the check was automatic and there was an error we ignore it.
        return;
    }
    QString messageTag = reply->readLine();
    if (messageTag.startsWith("WSCNoUpdateMessage"))
    {
        //There is no message to show
        if (manualRequest)
        {
            //As the request was manual show conformation that the check was sucsessfull.
            QMessageBox msgBox;
            msgBox.setWindowTitle("Word Search Creator");
            msgBox.setText("There are no updates for Word Search Creator at this time.");
            msgBox.exec();
            return;
        }
        else
            return;
    }
    //Simple check to stop harmless browser intecepts from displaying as an update message (e.g. wifi login pages).
    if (!messageTag.startsWith("WSCUpdateMessage"))
    {
        //If the request was manual then show the website
        if (manualRequest)
            QDesktopServices::openUrl(QUrl(QString("http://wordsearchcreator.org/update/").append(getVersion())));
        return;
    }

    //Read the update message.
    QString updateMessage = reply->readAll();
    //Construct a message box that can have clickable links.
    QDialog msgBox;
    msgBox.setWindowTitle("Word Search Creator");
    QVBoxLayout layout(&msgBox);
    QLabel label(&msgBox);
    label.setText(QString(updateMessage));
    label.setOpenExternalLinks(true);
    layout.addWidget(&label);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok, Qt::Horizontal, &msgBox);
    connect(&buttonBox, SIGNAL(accepted()), &msgBox, SLOT(close()));
    layout.addWidget(&buttonBox);
    layout.setSizeConstraint(QLayout::SetFixedSize);
    msgBox.exec();
}
