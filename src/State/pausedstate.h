#ifndef PAUSEDSTATE_H
#define PAUSEDSTATE_H

#include "state.h"

class PausedState : public State
{
public:
    PausedState(AudioRecorder* context);

    void toggleRecord() override;
    void togglePause() override;
    void onStateChanged() override;
};

#endif // PAUSEDSTATE_H
