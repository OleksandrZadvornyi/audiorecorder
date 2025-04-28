#ifndef LOGFILEOBSERVER_H
#define LOGFILEOBSERVER_H

#include "observer.h"
#include <QFile>
#include <QTextStream>

class RecordingFacade;

// Observer that logs recording state changes to a file
class LogFileObserver : public Observer
{
public:
    LogFileObserver(RecordingFacade* subject, const QString& logFilePath);
    ~LogFileObserver();

    void update() override;

private:
    RecordingFacade* m_subject;
    QFile m_logFile;
    QTextStream m_textStream;
};

#endif // LOGFILEOBSERVER_H
