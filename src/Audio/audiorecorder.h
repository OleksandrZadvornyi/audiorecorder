#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QMainWindow>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QUrl>
#include "../Command/commandinvoker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class AudioRecorder;
}
class QAudioBuffer;
class QMediaDevices;
QT_END_NAMESPACE

class State;
class AudioLevel;
class RecordingFacade;
class EncodingStrategy;
class StatusObserver;
class RecordingTimeDisplay;
class LogFileObserver;
class CommandInvoker;

class AudioRecorder : public QMainWindow
{
    Q_OBJECT

public:
    AudioRecorder();
    ~AudioRecorder();

    void changeState(State* state)
    {
        this->state_ = state;
    }

    Ui::AudioRecorder* getUI() { return ui; }
    QMediaRecorder* getMediaRecorder();
    void initializeRecording();

    void clearAudioLevels();

    // Get command invoker for outside access
    CommandInvoker* getCommandInvoker() { return m_commandInvoker; }

    // Get facade object for outsied access
    RecordingFacade* getRecordingFacade() { return m_recordingFacade; }

public slots:
    void processBuffer(const QAudioBuffer &);

private slots:
    void init();
    void setOutputLocation();
    void togglePause();
    void toggleRecord();

    void onStateChanged();
    void displayErrorMessage();
    void onMediaFormatChanged();

    void updateDevices();
    void updateFormats();

    // New slots for command history
    void displayCommandHistory();
    void onCommandExecuted(const QString& commandName);

private:
    QMediaFormat selectedMediaFormat() const;

    Ui::AudioRecorder *ui = nullptr;

    QList<AudioLevel *> m_audioLevels;
    bool m_outputLocationSet = false;
    bool m_updatingFormats = false;
    QMediaDevices *m_mediaDevices = nullptr;

    EncodingStrategy* createEncodingStrategy() const;

    // Using RecordingFacade instead of direct access to components
    RecordingFacade *m_recordingFacade = nullptr;

    // State pattern components
    State* state_ = nullptr;

    // Strategy pattern components
    EncodingStrategy* m_encodingStrategy = nullptr;

    // Observer pattern components
    StatusObserver* m_statusObserver = nullptr;
    RecordingTimeDisplay* m_timeDisplay = nullptr;
    LogFileObserver* m_logObserver = nullptr;

    // Command pattern components
    CommandInvoker* m_commandInvoker = nullptr;
};

#endif // AUDIORECORDER_H
