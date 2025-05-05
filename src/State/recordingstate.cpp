#include "recordingstate.h"
#include "../Audio/audiorecorder.h"
#include "../Command/recordercommands.h"
#include "pausedstate.h"
#include "stoppedstate.h"
#include "ui_audiorecorder.h"

RecordingState::RecordingState(AudioRecorder* context)
{
    this->context = context;
}

void RecordingState::toggleRecord()
{
    // Change state to stopped
    context->changeState(new StoppedState(context));

    // Create and execute stop recording command
    auto stopCommand = std::make_shared<StopRecordingCommand>(
        context->getRecordingFacade()
    );
    context->getCommandInvoker()->executeCommand(stopCommand);
}

void RecordingState::togglePause()
{
    // Change state to paused
    context->changeState(new PausedState(context));

    // Create and execute pause recording command
    auto pauseCommand = std::make_shared<PauseRecordingCommand>(
        context->getRecordingFacade()
    );
    context->getCommandInvoker()->executeCommand(pauseCommand);
}

void RecordingState::onStateChanged()
{
    Ui::AudioRecorder* ui = context->getUI();

    ui->recordButton->setText(AudioRecorder::tr("Stop"));
    ui->pauseButton->setText(AudioRecorder::tr("Pause"));

    ui->pauseButton->setEnabled(true);
    if (context->getMediaRecorder()->error() == QMediaRecorder::NoError) {
        ui->statusbar->showMessage(
            AudioRecorder::tr("Recording to %1").arg(
                context->getMediaRecorder()->actualLocation().toString()
            )
        );
    }
}
