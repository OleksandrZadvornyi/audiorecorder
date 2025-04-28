#ifndef RECORDINGSTATE_H
#define RECORDINGSTATE_H

#include "state.h"

class RecordingState : public State
{
public:
    RecordingState(AudioRecorder* context);

    void toggleRecord() override;
    void togglePause() override;
    void onStateChanged() override;
};

#endif // RECORDINGSTATE_H
