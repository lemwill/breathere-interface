/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainWindow.h"

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QFile>
//#include <QSerialPort>

#ifdef Q_OS_MAC
#include <CoreFoundation/CoreFoundation.h>
#endif

// On MacOSX, App nap prevents timer to be precise when app is in background.
void disableAppNap(){
#ifdef Q_OS_OSX
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    if( mainBundle ){
        CFMutableDictionaryRef infoDict = (CFMutableDictionaryRef) CFBundleGetInfoDictionary(mainBundle);
        if( infoDict ){
            CFDictionarySetValue(infoDict, CFSTR("LSUIElement"), CFSTR("1"));
        }
    }
#endif
}

int main(int argc, char *argv[])
{
    disableAppNap();

    QApplication a(argc, argv);

    QFile styleFile( ":/stylesheet/dark_stylesheet.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    a.setStyleSheet( style );

    MainWindow w;
    w.show();
    
    return a.exec();
}
