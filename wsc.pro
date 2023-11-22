# /***************************************************************************
# * Copyright (C) 2006-2023 M Wellings *
# * info@openforeveryone.co.uk *
# * *
# * This program is free software; you can redistribute it and/or modify *
# * it under the terms of the GNU General Public License version 2 as *
# * published by the Free Software Foundation *
# * *
# * This program is distributed in the hope that it will be useful, *
# * but WITHOUT ANY WARRANTY; without even the implied warranty of *
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
# * GNU General Public License for more details. *
# * *
# * You should have received a copy of the GNU General Public License *
# * along with this program; if not, write to the *
# * Free Software Foundation, Inc., *
# * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
# ***************************************************************************/

TEMPLATE = app

macx {
TARGET = WordSearchCreator
}else {
TARGET = wordsearchcreator
}

QT += xml
QT += network
QT += gui
QT += widgets
QT += printsupport
QT += svg

target.path = /usr/bin
desktop.path = /usr/share/applications
desktop.files += ./wordsearchcreator.desktop
icon256.path = /usr/share/icons/hicolor/256x256/apps/
icon256.files += ./images/wordsearchcreator.png
mime.path = /usr/share/mime/packages/
mime.files += ./wordsearchcreator-mime.xml
mimeicon.path = /usr/share/icons/hicolor/256x256/mimetypes/
mimeicon.files += ./images/oxygen/application-wordsearch.png
INSTALLS += target desktop icon256 mime mimeicon

win32 {
RC_FILE = wsc.rc
}

# Input
HEADERS += appearancedialog.h \
    directionselector.h \
    directionselectordialogue.h \
    listcreator.h \
    mainwindow.h \
    maskdialogue.h \
    maskdrawer.h \
    pagelayoutdialogue.h \
    wordsearch.h \
    wordsearchcontrol.h \
    wordsearchdrawer.h \
    alphabetdialogue.h \
    wordsearchdoc.h \
    wordsearchapplication.h \
    updatechecker.h \
    preferencesdialog.h
FORMS += appearance.ui \
    listcreator.ui \
    pagelayout.ui \
    alphabet.ui \
    wordsearchcontrol.ui \
    preferencesdialog.ui
SOURCES += appearancedialog.cpp \
    directionselector.cpp \
    directionselectordialogue.cpp \
    listcreator.cpp \
    main.cpp \
    mainwindow.cpp \
    maskdialogue.cpp \
    maskdrawer.cpp \
    pagelayoutdialogue.cpp \
    wordsearch.cpp \
    wordsearchcontrol.cpp \
    wordsearchdrawer.cpp \
    alphabetdialogue.cpp \
    wordsearchdoc.cpp \
    wordsearchapplication.cpp \
    updatechecker.cpp \
    preferencesdialog.cpp
RESOURCES += wsc.qrc
