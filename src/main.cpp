// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "./Audio/audiorecorder.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AudioRecorder recorder;
    recorder.show();

    return app.exec();
}
