#include "pausedstate.h"
#include "../Audio/audiorecorder.h"
#include "../Command/recordercommands.h"
#include "recordingstate.h"
#include "stoppedstate.h"
#include "ui_audiorecorder.h"

PausedState::PausedState(AudioRecorder* context)
{
    this->context = context;
}

void PausedState::toggleRecord()
{
    // Change state to stopped
    context->changeState(new StoppedState(context));

    // Create and execute stop recording command
    auto stopCommand = std::make_shared<StopRecordingCommand>(
        context->getRecordingFacade()
    );
    context->getCommandInvoker()->executeCommand(stopCommand);
}

void PausedState::togglePause()
{
    // Change state to recording
    context->changeState(new RecordingState(context));

    // Create and execute resume recording command
    auto resumeCommand = std::make_shared<ResumeRecordingCommand>(
        context->getRecordingFacade()
    );
    context->getCommandInvoker()->executeCommand(resumeCommand);
}

void PausedState::onStateChanged()
{
    Ui::AudioRecorder* ui = context->getUI();

    context->clearAudioLevels();
    ui->recordButton->setText(AudioRecorder::tr("Stop"));
    ui->pauseButton->setText(AudioRecorder::tr("Resume"));

    ui->pauseButton->setEnabled(true);
    if (context->getMediaRecorder()->error() == QMediaRecorder::NoError)
        ui->statusbar->showMessage(AudioRecorder::tr("Paused"));
}
