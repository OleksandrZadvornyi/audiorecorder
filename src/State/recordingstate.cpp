#include "recordingstate.h"
#include "../Audio/audiorecorder.h"
#include "pausedstate.h"
#include "stoppedstate.h"
#include "ui_audiorecorder.h"

RecordingState::RecordingState(AudioRecorder* context)
{
    this->context = context;
}

void RecordingState::onStateChanged()
{
    Ui::AudioRecorder* ui = context->getUI();
    QString statusMessage;

    statusMessage = AudioRecorder::tr("Recording to %1").arg(
        context->getMediaRecorder()->actualLocation().toString()
    );
    ui->recordButton->setText(AudioRecorder::tr("Stop"));
    ui->pauseButton->setText(AudioRecorder::tr("Pause"));

    ui->pauseButton->setEnabled(context->getMediaRecorder()->recorderState() != QMediaRecorder::StoppedState);
    if (context->getMediaRecorder()->error() == QMediaRecorder::NoError)
        ui->statusbar->showMessage(statusMessage);
}

void RecordingState::toggleRecord()
{
    context->changeState(new StoppedState(context));
    context->getMediaRecorder()->stop();
}

void RecordingState::togglePause()
{
    context->changeState(new PausedState(context));
    context->getMediaRecorder()->pause();
}
