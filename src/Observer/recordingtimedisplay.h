#ifndef RECORDINGTIMEDISPLAY_H
#define RECORDINGTIMEDISPLAY_H

#include "observer.h"
#include <QLabel>

class RecordingFacade;

// Observer that updates a label with the current recording duration
class RecordingTimeDisplay : public Observer, QObject
{
public:
    RecordingTimeDisplay(RecordingFacade* subject, QLabel* label);
    ~RecordingTimeDisplay();

    void update() override;

private:
    RecordingFacade* m_subject;
    QLabel* m_timeLabel;
    QTimer* m_timer;

public slots:
    void updateDisplay();
};

#endif // RECORDINGTIMEDISPLAY_H
