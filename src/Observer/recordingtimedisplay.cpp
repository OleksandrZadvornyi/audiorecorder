#include "recordingtimedisplay.h"
#include "../Facade/recordingfacade.h"
#include <QTimer>

RecordingTimeDisplay::RecordingTimeDisplay(RecordingFacade* subject, QLabel* label)
    : m_subject(subject), m_timeLabel(label)
{
    if (m_subject) {
        m_subject->attach(this);
    }

    // Create timer for updating the display
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
}

RecordingTimeDisplay::~RecordingTimeDisplay()
{
    if (m_subject) {
        m_subject->detach(this);
    }

    m_timer->stop();
    delete m_timer;
}

void RecordingTimeDisplay::update()
{
    // Start or stop the timer based on recording state
    if (m_subject->getStatus() == RecordingFacade::Recording) {
        if (!m_timer->isActive()) {
            m_timer->start(1000); // Update once per second
        }
    } else {
        m_timer->stop();

        // If stopped or error, clear the display
        if (m_subject->getStatus() == RecordingFacade::Stopped ||
            m_subject->getStatus() == RecordingFacade::Error) {
            m_timeLabel->setText("00:00:00");
        }
    }

    // Update display immediately
    updateDisplay();
}

void RecordingTimeDisplay::updateDisplay()
{
    if (!m_subject || !m_timeLabel)
        return;

    qint64 duration = m_subject->duration() / 1000; // Convert to seconds
    int hours = duration / 3600;
    int minutes = (duration % 3600) / 60;
    int seconds = duration % 60;

    m_timeLabel->setText(QString("%1:%2:%3")
                             .arg(hours, 2, 10, QChar('0'))
                             .arg(minutes, 2, 10, QChar('0'))
                             .arg(seconds, 2, 10, QChar('0')));
}
