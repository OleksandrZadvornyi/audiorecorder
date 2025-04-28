#ifndef ENCODINGSTRATEGY_H
#define ENCODINGSTRATEGY_H

#include <QMediaRecorder>

// Strategy interface for encoding modes
class EncodingStrategy
{
public:
    virtual ~EncodingStrategy() = default;
    virtual void applyEncodingSettings(QMediaRecorder* recorder) const = 0;
};

// Concrete strategy for constant quality encoding
class ConstantQualityStrategy : public EncodingStrategy
{
public:
    explicit ConstantQualityStrategy(int quality) : m_quality(quality) {}

    void applyEncodingSettings(QMediaRecorder* recorder) const override {
        recorder->setQuality(QMediaRecorder::Quality(m_quality));
        recorder->setEncodingMode(QMediaRecorder::ConstantQualityEncoding);
    }

private:
    int m_quality;
};

// Concrete strategy for constant bit rate encoding
class ConstantBitRateStrategy : public EncodingStrategy
{
public:
    explicit ConstantBitRateStrategy(int bitRate) : m_bitRate(bitRate) {}

    void applyEncodingSettings(QMediaRecorder* recorder) const override {
        recorder->setAudioBitRate(m_bitRate);
        recorder->setEncodingMode(QMediaRecorder::ConstantBitRateEncoding);
    }

private:
    int m_bitRate;
};

#endif // ENCODINGSTRATEGY_H
