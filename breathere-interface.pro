QT += charts bluetooth multimedia \
    widgets
#serialport


QMAKE_TARGET_BUNDLE_PREFIX = williamlemaire.test

HEADERS += \
    bluetoothbaseclass.h \
    configurealarmdialog.h \
    connectionhandler.h \
    devicefinder.h \
    devicehandler.h \
    deviceinfo.h \
    heartrate-global.h \
    widget.h \
    xyseriesiodevice.h

SOURCES += \
    bluetoothbaseclass.cpp \
    configurealarmdialog.cpp \
    connectionhandler.cpp \
    devicefinder.cpp \
    devicehandler.cpp \
    deviceinfo.cpp \
    main.cpp\
    widget.cpp \
    xyseriesiodevice.cpp

INSTALLS += target
ios{
    QMAKE_INFO_PLIST = Info.plist
}

APP_Sounds.files = $$PWD/sounds/beep.mp3
APP_Sounds.files = sounds
QMAKE_BUNDLE_DATA += APP_Sounds


DISTFILES += \
    dark_stylesheet.qss

RESOURCES += \
    ressource.qrc
