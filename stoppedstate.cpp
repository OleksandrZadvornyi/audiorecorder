#include "stoppedstate.h"
#include "audiorecorder.h"
#include "recordingstate.h"
#include "pausedstate.h"
#include "ui_audiorecorder.h"

StoppedState::StoppedState(AudioRecorder* context)
{
    this->context = context;
}

void StoppedState::toggleRecord()
{
    context->changeState(new RecordingState(context));
    context->getMediaRecorder()->record();
    context->initializeRecording();
}

void StoppedState::togglePause()
{
    context->changeState(new PausedState(context));
    context->getMediaRecorder()->pause();
}

void StoppedState::onStateChanged()
{
    Ui::AudioRecorder* ui = context->getUI();
    QString statusMessage;

    context->clearAudioLevels();
    statusMessage = AudioRecorder::tr("Stopped");
    ui->recordButton->setText(AudioRecorder::tr("Record"));
    ui->pauseButton->setText(AudioRecorder::tr("Pause"));

    ui->pauseButton->setEnabled(context->getMediaRecorder()->recorderState() != QMediaRecorder::StoppedState);
    if (context->getMediaRecorder()->error() == QMediaRecorder::NoError)
        ui->statusbar->showMessage(statusMessage);
}


