#include "mainWindow.h"
#include <QtWidgets/QApplication>
#include <QFile>

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
