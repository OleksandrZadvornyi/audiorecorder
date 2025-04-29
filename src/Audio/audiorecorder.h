#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QMainWindow>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QUrl>

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

private:
    EncodingStrategy* m_encodingStrategy = nullptr;

    QMediaFormat selectedMediaFormat() const;

    Ui::AudioRecorder *ui = nullptr;

    // Using RecordingFacade instead of direct access to components
    RecordingFacade *m_recordingFacade = nullptr;
    QMediaDevices *m_mediaDevices = nullptr;

    QList<AudioLevel *> m_audioLevels;
    bool m_outputLocationSet = false;
    bool m_updatingFormats = false;

    State* state_ = nullptr;
    EncodingStrategy* createEncodingStrategy() const;

    // Observer pattern components
    StatusObserver* m_statusObserver = nullptr;
    RecordingTimeDisplay* m_timeDisplay = nullptr;
    LogFileObserver* m_logObserver = nullptr;
};

#endif // AUDIORECORDER_H
