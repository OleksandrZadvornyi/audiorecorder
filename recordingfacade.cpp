#include "recordingfacade.h"
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include "encodingstrategy.h"

RecordingFacade::RecordingFacade(QObject *parent)
    : QObject(parent)
{
    // Initialize the recording components
    m_recorder = new QMediaRecorder(this);
    m_captureSession = new QMediaCaptureSession(this);
    m_audioInput = new QAudioInput(this);

    // Configure the capture session
    m_captureSession->setRecorder(m_recorder);
    m_captureSession->setAudioInput(m_audioInput);

    // Connect signals
    connect(m_recorder, &QMediaRecorder::durationChanged,
            this, &RecordingFacade::durationChanged);
    connect(m_recorder, &QMediaRecorder::recorderStateChanged,
            this, &RecordingFacade::stateChanged);
    connect(m_recorder, &QMediaRecorder::errorChanged,
            this, &RecordingFacade::errorOccurred);
}

RecordingFacade::~RecordingFacade()
{
    // QObject parent-child relationship will handle cleanup
}

void RecordingFacade::startRecording(
    const QAudioDevice &device,
    int sampleRate,
    int bitRate,
    int channelCount,
    int quality,
    bool constantQuality,
    const QMediaFormat &format,
    const QUrl &outputLocation,
    const EncodingStrategy *strategy)
{
    // Setup recording parameters
    m_audioInput->setDevice(device);
    m_recorder->setMediaFormat(format);
    m_recorder->setAudioSampleRate(sampleRate);
    m_recorder->setAudioChannelCount(channelCount);

    // Apply strategy if provided, otherwise use the default approach
    if (strategy) {
        strategy->applyEncodingSettings(m_recorder);
    } else {
        m_recorder->setAudioBitRate(bitRate);
        m_recorder->setQuality(QMediaRecorder::Quality(quality));
        m_recorder->setEncodingMode(
            constantQuality ? QMediaRecorder::ConstantQualityEncoding
                            : QMediaRecorder::ConstantBitRateEncoding);
    }

    // Set output location if provided
    if (!outputLocation.isEmpty()) {
        m_recorder->setOutputLocation(outputLocation);
    }

    // Start recording
    m_recorder->record();
}

void RecordingFacade::pauseRecording()
{
    m_recorder->pause();
}

void RecordingFacade::resumeRecording()
{
    m_recorder->record();
}

void RecordingFacade::stopRecording()
{
    m_recorder->stop();
}

bool RecordingFacade::isRecording() const
{
    return m_recorder->recorderState() == QMediaRecorder::RecordingState;
}

bool RecordingFacade::isPaused() const
{
    return m_recorder->recorderState() == QMediaRecorder::PausedState;
}

bool RecordingFacade::isStopped() const
{
    return m_recorder->recorderState() == QMediaRecorder::StoppedState;
}

QMediaRecorder* RecordingFacade::recorder() const
{
    return m_recorder;
}

QMediaCaptureSession* RecordingFacade::captureSession() const
{
    return m_captureSession;
}

QString RecordingFacade::errorString() const
{
    return m_recorder->errorString();
}

qint64 RecordingFacade::duration() const
{
    return m_recorder->duration();
}
