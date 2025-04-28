#ifndef STATUSOBSERVER_H
#define STATUSOBSERVER_H

#include "observer.h"
#include <QStatusBar>
#include <QString>

class RecordingFacade;

// Observer that updates a status bar with the current recording status
class StatusObserver : public Observer
{
public:
    StatusObserver(RecordingFacade* subject, QStatusBar* statusBar);
    ~StatusObserver();

    void update() override;

private:
    RecordingFacade* m_subject;
    QStatusBar* m_statusBar;
};

#endif // STATUSOBSERVER_H
