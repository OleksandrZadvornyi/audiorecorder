#ifndef RECORDINGTIMEDISPLAY_H
#define RECORDINGTIMEDISPLAY_H

#include "observer.h"
#include <QStatusBar>

class RecordingFacade;

// Observer that updates a label with the current recording duration
class RecordingTimeDisplay : public QObject, Observer
{
    Q_OBJECT

public:
    RecordingTimeDisplay(RecordingFacade* subject, QStatusBar* statusBar);
    ~RecordingTimeDisplay();

    void update() override;

private:
    RecordingFacade* m_subject;
    QStatusBar* m_statusBar;
    QTimer* m_timer;

public slots:
    void updateDisplay();
};

#endif // RECORDINGTIMEDISPLAY_H
