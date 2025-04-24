#include "pausedstate.h"
#include "audiorecorder.h"
#include "recordingstate.h"
#include "stoppedstate.h"
#include "ui_audiorecorder.h"

PausedState::PausedState(AudioRecorder* context)
{
    this->context = context;
}

void PausedState::toggleRecord()
{
    context->changeState(new StoppedState(context));
    context->getMediaRecorder()->stop();
}

void PausedState::togglePause()
{
    context->changeState(new RecordingState(context));
    context->getMediaRecorder()->record();
}

void PausedState::onStateChanged()
{
    Ui::AudioRecorder* ui = context->getUI();
    QString statusMessage;

    context->clearAudioLevels();
    statusMessage = AudioRecorder::tr("Paused");
    ui->recordButton->setText(AudioRecorder::tr("Stop"));
    ui->pauseButton->setText(AudioRecorder::tr("Resume"));

    ui->pauseButton->setEnabled(context->getMediaRecorder()->recorderState() != QMediaRecorder::StoppedState);
    if (context->getMediaRecorder()->error() == QMediaRecorder::NoError)
        ui->statusbar->showMessage(statusMessage);
}
