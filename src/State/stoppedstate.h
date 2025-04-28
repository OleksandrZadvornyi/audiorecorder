#ifndef STOPPEDSTATE_H
#define STOPPEDSTATE_H

#include "state.h"

class StoppedState : public State
{
public:
    StoppedState(AudioRecorder* context);

    void toggleRecord() override;
    void togglePause() override;
    void onStateChanged() override;
};

#endif // STOPPEDSTATE_H
