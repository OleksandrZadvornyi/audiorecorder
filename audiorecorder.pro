TEMPLATE = app
TARGET = audiorecorder

QT += multimedia

win32:INCLUDEPATH += $$PWD

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
    src/Observer/observer.h \
    src/Observer/subject.h \
    src/Observer/logfileobserver.h \
    src/Observer/statusobserver.h \
    src/Observer/recordingtimedisplay.h \
    src/State/state.h \
    src/State/pausedstate.h \
    src/State/recordingstate.h \
    src/State/stoppedstate.h \
    src/Facade/recordingfacade.h 

FORMS += \
    ui/audiorecorder.ui

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/audiorecorder
INSTALLS += target

QT+=widgets
#include(../shared/shared.pri)
