#include "statusobserver.h"
#include "../Facade/recordingfacade.h"

StatusObserver::StatusObserver(RecordingFacade* subject, QStatusBar* statusBar)
    : m_subject(subject), m_statusBar(statusBar)
{
    if (m_subject) {
        m_subject->attach(this);
    }
}

StatusObserver::~StatusObserver()
{
    if (m_subject) {
        m_subject->detach(this);
    }
}

void StatusObserver::update()
{
    if (!m_subject || !m_statusBar)
        return;

    // Update status bar text based on recording status
    switch (m_subject->getStatus()) {
    case RecordingFacade::Recording:
        m_statusBar->showMessage(QObject::tr("Recording to %1").arg(
            m_subject->recorder()->actualLocation().toString()));
        break;
    case RecordingFacade::Paused:
        m_statusBar->showMessage(QObject::tr("Paused"));
        break;
    case RecordingFacade::Stopped:
        m_statusBar->showMessage(QObject::tr("Stopped"));
        break;
    case RecordingFacade::Error:
        m_statusBar->showMessage(QObject::tr("Error: %1").arg(m_subject->errorString()));
        break;
    default:
        m_statusBar->showMessage(QObject::tr("Ready"));
        break;
    }
}
