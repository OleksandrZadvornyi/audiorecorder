#include "audiorecorder.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AudioRecorder recorder;
    recorder.show();

    return app.exec();
}
