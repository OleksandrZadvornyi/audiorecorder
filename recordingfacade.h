#ifndef RECORDINGFACADE_H
#define RECORDINGFACADE_H

#include <QObject>
#include <QAudioDevice>
#include <QMediaFormat>
#include <QString>
#include <QUrl>

class QMediaRecorder;
class QMediaCaptureSession;
class QAudioInput;

// RecordingFacade provides a simplified interface to the complex
// audio recording subsystem
class RecordingFacade : public QObject
{
    Q_OBJECT

public:
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
                        const QUrl &outputLocation = QUrl());

    void pauseRecording();
    void resumeRecording();
    void stopRecording();

    // Status methods
    bool isRecording() const;
    bool isPaused() const;
    bool isStopped() const;

    // Getters
    QMediaRecorder* recorder() const;
    QString errorString() const;
    qint64 duration() const;

signals:
    void durationChanged(qint64 duration);
    void stateChanged();
    void errorOccurred();

private:
    QMediaRecorder *m_recorder;
    QMediaCaptureSession *m_captureSession;
    QAudioInput *m_audioInput;
};

#endif // RECORDINGFACADE_H
