#include "logfileobserver.h"
#include "../Facade/recordingfacade.h"
#include <QDateTime>

LogFileObserver::LogFileObserver(RecordingFacade* subject, const QString& logFilePath)
    : m_subject(subject), m_logFile(logFilePath)
{
    if (m_subject) {
        m_subject->attach(this);
    }

    // Open the log file
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_textStream.setDevice(&m_logFile);
    }
}

LogFileObserver::~LogFileObserver()
{
    if (m_subject) {
        m_subject->detach(this);
    }

    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

void LogFileObserver::update()
{
    if (!m_subject || !m_logFile.isOpen())
        return;

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString status;

    // Get status as string
    switch (m_subject->getStatus()) {
    case RecordingFacade::Recording:
        status = "Recording started";
        break;
    case RecordingFacade::Paused:
        status = "Recording paused";
        break;
    case RecordingFacade::Stopped:
        status = "Recording stopped";
        break;
    case RecordingFacade::Error:
        status = "Error: " + m_subject->errorString();
        break;
    default:
        status = "Unknown status";
        break;
    }

    // Write to log file
    m_textStream << timestamp << " - " << status << "\n";
    m_textStream.flush();
}
