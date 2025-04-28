#ifndef STATE_H
#define STATE_H

class AudioRecorder;

class State
{
protected:
    AudioRecorder* context;

public:
    virtual ~State() = default;

    void setContext(AudioRecorder* context)
    {
        this->context = context;
    }

    virtual void toggleRecord() = 0;
    virtual void togglePause() = 0;
    virtual void onStateChanged() = 0;
};

#endif // STATE_H
