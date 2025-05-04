#ifndef RECORDERCOMMANDS_H
#define RECORDERCOMMANDS_H

#include "command.h"
#include "../Facade/recordingfacade.h"
#include <QAudioDevice>
#include <QMediaFormat>
#include <QUrl>

// Start Recording Command
class StartRecordingCommand : public Command
{
private:
    RecordingFacade* m_facade;
    QAudioDevice m_device;
    int m_sampleRate;
    int m_bitRate;
    int m_channelCount;
    int m_quality;
    bool m_constantQuality;
    QMediaFormat m_format;
    QUrl m_outputLocation;
    const EncodingStrategy* m_strategy;

public:
    StartRecordingCommand(RecordingFacade* facade,
                          const QAudioDevice& device,
                          int sampleRate,
                          int bitRate,
                          int channelCount,
                          int quality,
                          bool constantQuality,
                          const QMediaFormat& format,
                          const QUrl& outputLocation = QUrl(),
                          const EncodingStrategy* strategy = nullptr)
        : m_facade(facade),
        m_device(device),
        m_sampleRate(sampleRate),
        m_bitRate(bitRate),
        m_channelCount(channelCount),
        m_quality(quality),
        m_constantQuality(constantQuality),
        m_format(format),
        m_outputLocation(outputLocation),
        m_strategy(strategy)
    {}

    void execute() override {
        m_facade->startRecording(m_device, m_sampleRate, m_bitRate, m_channelCount,
                                 m_quality, m_constantQuality, m_format,
                                 m_outputLocation, m_strategy);
    }

    QString getName() const override {
        return "Start Recording";
    }
};

// Pause Recording Command
class PauseRecordingCommand : public Command
{
private:
    RecordingFacade* m_facade;

public:
    explicit PauseRecordingCommand(RecordingFacade* facade)
        : m_facade(facade)
    {}

    void execute() override {
        m_facade->pauseRecording();
    }

    QString getName() const override {
        return "Pause Recording";
    }
};

// Resume Recording Command
class ResumeRecordingCommand : public Command
{
private:
    RecordingFacade* m_facade;

public:
    explicit ResumeRecordingCommand(RecordingFacade* facade)
        : m_facade(facade)
    {}

    void execute() override {
        m_facade->resumeRecording();
    }

    QString getName() const override {
        return "Resume Recording";
    }
};

// Stop Recording Command
class StopRecordingCommand : public Command
{
private:
    RecordingFacade* m_facade;

public:
    explicit StopRecordingCommand(RecordingFacade* facade)
        : m_facade(facade)
    {}

    void execute() override {
        m_facade->stopRecording();
    }

    QString getName() const override {
        return "Stop Recording";
    }
};

#endif // RECORDERCOMMANDS_H
