QT += charts bluetooth multimedia \
    widgets
#serialport


QMAKE_TARGET_BUNDLE_PREFIX = williamlemaire.test

HEADERS += \
    src/bluetooth/bluetoothbaseclass.h \
    src/bluetooth/connectionhandler.h \
    src/bluetooth/devicefinder.h \
    src/bluetooth/devicehandler.h \
    src/bluetooth/deviceinfo.h \
    src/bluetooth/heartrate-global.h \
    src/chartwidget.h \
    src/configurealarmdialog.h \
    src/mainWindow.h

SOURCES += \
    src/bluetooth/bluetoothbaseclass.cpp \
    src/bluetooth/connectionhandler.cpp \
    src/bluetooth/devicefinder.cpp \
    src/bluetooth/devicehandler.cpp \
    src/bluetooth/deviceinfo.cpp \
    src/chartwidget.cpp \
    src/configurealarmdialog.cpp \
    src/main.cpp \
    src/mainWindow.cpp

INSTALLS += target
ios{
    QMAKE_INFO_PLIST = Info.plist
}

APP_Sounds.files = $$PWD/sounds/beep.mp3
APP_Sounds.files = sounds
QMAKE_BUNDLE_DATA += APP_Sounds

macx: QMAKE_LFLAGS += -F /System/Library/Frameworks/CoreFoundation.framework/
macx: LIBS += -framework CoreFoundation


DISTFILES += \
    dark_stylesheet.qss

RESOURCES += \
    ressource.qrc
