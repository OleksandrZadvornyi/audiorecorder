#include "stoppedstate.h"
#include "../Audio/audiorecorder.h"
#include "recordingstate.h"
#include "pausedstate.h"
#include "ui_audiorecorder.h"

StoppedState::StoppedState(AudioRecorder* context)
{
    this->context = context;
}

void StoppedState::toggleRecord()
{
    // Change state to recording
    context->changeState(new RecordingState(context));

    // Create and execute the start recording command
    context->initializeRecording();
}

void StoppedState::togglePause()
{
    // Do nothing in stopped state
}

void StoppedState::onStateChanged()
{
    Ui::AudioRecorder* ui = context->getUI();

    context->clearAudioLevels();
    ui->recordButton->setText(AudioRecorder::tr("Record"));
    ui->pauseButton->setText(AudioRecorder::tr("Pause"));

    ui->pauseButton->setEnabled(false);
    if (context->getMediaRecorder()->error() == QMediaRecorder::NoError)
        ui->statusbar->showMessage(AudioRecorder::tr("Stopped"));
}


