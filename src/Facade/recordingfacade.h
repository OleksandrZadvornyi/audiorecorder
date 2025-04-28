#ifndef RECORDINGFACADE_H
#define RECORDINGFACADE_H

#include <QObject>
#include <QAudioDevice>
#include <QMediaFormat>
#include <QString>
#include <QUrl>
#include <QMediaRecorder>
#include "subject.h"

class QMediaRecorder;
class QMediaCaptureSession;
class QAudioInput;
class EncodingStrategy;

// RecordingFacade provides a simplified interface to the complex
// audio recording subsystem and now acts as a Subject for Observers
class RecordingFacade : public QObject, public Subject
{
    Q_OBJECT

public:
    // Recording states that observers might be interested in
    enum RecordingStatus {
        Idle,
        Recording,
        Paused,
        Stopped,
        Error
    };

    explicit RecordingFacade(QObject *parent = nullptr);
    ~RecordingFacade();

    // Simple interface methods
    void startRecording(const QAudioDevice &device,
                        int sampleRate,
                        int bitRate,
                        int channelCount,
                        int quality,
                        bool constantQuality,
                        const QMediaFormat &format,
                        const QUrl &outputLocation = QUrl(),
                        const EncodingStrategy *strategy = nullptr);

    void pauseRecording();
    void resumeRecording();
    void stopRecording();

    // Status methods
    bool isRecording() const;
    bool isPaused() const;
    bool isStopped() const;
    RecordingStatus getStatus() const { return m_status; }

    // Getters
    QMediaRecorder* recorder() const;
    QMediaCaptureSession* captureSession() const;
    QString errorString() const;
    qint64 duration() const;

signals:
    void durationChanged(qint64 duration);
    void stateChanged();
    void errorOccurred();

private slots:
    void handleStateChange();
    void handleError();

private:
    QMediaRecorder *m_recorder;
    QMediaCaptureSession *m_captureSession;
    QAudioInput *m_audioInput;
    RecordingStatus m_status;
};

#endif // RECORDINGFACADE_H
