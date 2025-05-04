TEMPLATE = app
TARGET = audiorecorder

QT += core gui multimedia widgets

CONFIG += c++17

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/main.cpp \
    src/Audio/audiolevel.cpp \
    src/Audio/audiorecorder.cpp \
    src/Observer/logfileobserver.cpp \
    src/Observer/statusobserver.cpp \
    src/Observer/recordingtimedisplay.cpp \
    src/State/pausedstate.cpp \
    src/State/recordingstate.cpp \
    src/State/stoppedstate.cpp \
    src/Facade/recordingfacade.cpp 

HEADERS += \
    src/Audio/audiolevel.h \
    src/Audio/audiorecorder.h \
    src/Audio/encodingstrategy.h \
    src/Command/commandinvoker.h \
    src/Command/recordercommands.h \
    src/Observer/observer.h \
    src/Observer/subject.h \
    src/Observer/logfileobserver.h \
    src/Observer/statusobserver.h \
    src/Observer/recordingtimedisplay.h \
    src/State/state.h \
    src/State/pausedstate.h \
    src/State/recordingstate.h \
    src/State/stoppedstate.h \
    src/Facade/recordingfacade.h \
    src/Command/command.h

FORMS += \
    ui/audiorecorder.ui

macx {
    QMAKE_INFO_PLIST = Info.plist.in
}
