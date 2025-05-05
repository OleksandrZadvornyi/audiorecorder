#include "audiorecorder.h"
#include "audiolevel.h"
#include "ui_audiorecorder.h"
#include "../State/stoppedstate.h"
#include "../Facade/recordingfacade.h"
#include "encodingstrategy.h"
#include "../Observer/statusobserver.h"
#include "../Observer/recordingtimedisplay.h"
#include "../Observer/logfileobserver.h"
#include "../Command/commandinvoker.h"
#include "../Command/recordercommands.h"

#include <QAudioBuffer>
#include <QAudioDevice>
#include <QAudioInput>
#include <QDir>
#include <QFileDialog>
#include <QImageCapture>
#include <QMediaDevices>
#include <QMediaFormat>
#include <QMediaRecorder>
#include <QMimeType>
#include <QStandardPaths>
#include <QToolBar>
#include <QListWidget>
#include <QDockWidget>
#include <memory>

#if QT_CONFIG(permissions)
#include <QPermission>
#endif

static QList<qreal> getBufferLevels(const QAudioBuffer &buffer);

AudioRecorder::AudioRecorder() : ui(new Ui::AudioRecorder)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/recorder_icon.ico"));
    changeState(new StoppedState(this));
    m_encodingStrategy = nullptr; // Initialize the strategy pointer

    // Create command invoker
    m_commandInvoker = new CommandInvoker(this);
    connect(m_commandInvoker, &CommandInvoker::commandExecuted,
            this, &AudioRecorder::onCommandExecuted);

    // Add Command History dock widget
    QDockWidget *historyDock = new QDockWidget(tr("Command History"), this);
    historyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QListWidget *historyList = new QListWidget(historyDock);
    historyDock->setWidget(historyList);
    addDockWidget(Qt::RightDockWidgetArea, historyDock);

    // channels
    ui->channelsBox->addItem(tr("Default"), QVariant(-1));
    ui->channelsBox->addItem(QStringLiteral("1"), QVariant(1));
    ui->channelsBox->addItem(QStringLiteral("2"), QVariant(2));
    ui->channelsBox->addItem(QStringLiteral("4"), QVariant(4));

    // quality
    ui->qualitySlider->setRange(0, int(QImageCapture::VeryHighQuality));
    ui->qualitySlider->setValue(int(QImageCapture::NormalQuality));

    // bit rates:
    ui->bitrateBox->addItem(tr("Default"), QVariant(0));
    ui->bitrateBox->addItem(QStringLiteral("32000"), QVariant(32000));
    ui->bitrateBox->addItem(QStringLiteral("64000"), QVariant(64000));
    ui->bitrateBox->addItem(QStringLiteral("96000"), QVariant(96000));
    ui->bitrateBox->addItem(QStringLiteral("128000"), QVariant(128000));

    // audio input initialization
    init();
}

AudioRecorder::~AudioRecorder()
{
    delete m_statusObserver;
    delete m_timeDisplay;
    delete m_logObserver;
    delete m_encodingStrategy;
    delete m_commandInvoker;
    delete ui;
}

static QVariant boxValue(const QComboBox *box)
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

// Method to create the appropriate strategy
EncodingStrategy* AudioRecorder::createEncodingStrategy() const
{
    if (ui->constantQualityRadioButton->isChecked()) {
        return new ConstantQualityStrategy(ui->qualitySlider->value());
    } else {
        return new ConstantBitRateStrategy(boxValue(ui->bitrateBox).toInt());
    }
}

void AudioRecorder::init()
{
#if QT_CONFIG(permissions)
    QMicrophonePermission microphonePermission;
    switch (qApp->checkPermission(microphonePermission)) {
    case Qt::PermissionStatus::Undetermined:
        qApp->requestPermission(microphonePermission, this, &AudioRecorder::init);
        return;
    case Qt::PermissionStatus::Denied:
        qWarning("Microphone permission is not granted!");
        return;
    case Qt::PermissionStatus::Granted:
        break;
    }
#endif

    // Create the RecordingFacade object
    m_recordingFacade = new RecordingFacade(this);

    // Create observers and attach them to the facade
    m_statusObserver = new StatusObserver(m_recordingFacade, ui->statusbar);

    // Create a label for displaying recording time
    ui->statusbar->showMessage(tr("Recorder is stopped"));
    m_timeDisplay = new RecordingTimeDisplay(m_recordingFacade, ui->statusbar);

    // Create log file observer
    QString logPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                      + "/recording_log.txt";
    m_logObserver = new LogFileObserver(m_recordingFacade, logPath);

    // Connect signals from facade to our slots
    connect(m_recordingFacade, &RecordingFacade::durationChanged,
            m_timeDisplay, &RecordingTimeDisplay::updateDisplay);
    connect(m_recordingFacade, &RecordingFacade::stateChanged,
            this, &AudioRecorder::onStateChanged);
    connect(m_recordingFacade, &RecordingFacade::errorOccurred,
            this, &AudioRecorder::displayErrorMessage);

    // audio devices
    m_mediaDevices = new QMediaDevices(this);
    connect(m_mediaDevices, &QMediaDevices::audioInputsChanged, this,
            &AudioRecorder::updateDevices);
    updateDevices();

    // audio codecs and container formats
    updateFormats();
    connect(ui->audioCodecBox, &QComboBox::currentIndexChanged, this,
            &AudioRecorder::updateFormats);
    connect(ui->containerBox, &QComboBox::currentIndexChanged, this, &AudioRecorder::updateFormats);

    // sample rate
    constexpr auto allSamplingRates = std::array{
        8000,  11025, 12000, 16000, 22050,  24000,  32000,  44100,
        48000, 64000, 88200, 96000, 128000, 176400, 192000,
    };

    QAudioDevice device;
    if (m_recordingFacade->recorder()) {
        device = m_recordingFacade->captureSession()->audioInput()->device();
        int minSamplingRate = device.minimumSampleRate();
        int maxSamplingRate = device.maximumSampleRate();

        for (int rate : allSamplingRates) {
            if (rate < minSamplingRate || rate > maxSamplingRate)
                continue;
            ui->sampleRateBox->addItem(QString::number(rate), rate);
        }
        int preferredRate = device.preferredFormat().sampleRate();
        if (preferredRate > 0) {
            int index = ui->sampleRateBox->findData(device.preferredFormat().sampleRate());
            ui->sampleRateBox->setCurrentIndex(index);
        }
    }
}

QMediaRecorder* AudioRecorder::getMediaRecorder()
{
    // Return the facade's recorder
    return m_recordingFacade->recorder();
}

void AudioRecorder::onStateChanged()
{
    this->state_->onStateChanged();
    displayCommandHistory(); // Update command history display
}

void AudioRecorder::toggleRecord()
{
    this->state_->toggleRecord();
}

void AudioRecorder::togglePause()
{
    this->state_->togglePause();
}

void AudioRecorder::initializeRecording()
{
    // Create a new encoding strategy
    delete m_encodingStrategy;
    m_encodingStrategy = createEncodingStrategy();

    // Create and execute the start recording command
    auto startCommand = std::make_shared<StartRecordingCommand>(
        m_recordingFacade,
        boxValue(ui->audioDeviceBox).value<QAudioDevice>(),
        boxValue(ui->sampleRateBox).toInt(),
        boxValue(ui->bitrateBox).toInt(),
        boxValue(ui->channelsBox).toInt(),
        ui->qualitySlider->value(),
        ui->constantQualityRadioButton->isChecked(),
        selectedMediaFormat(),
        m_outputLocationSet ? m_recordingFacade->recorder()->outputLocation() : QUrl(),
        m_encodingStrategy
        );

    m_commandInvoker->executeCommand(startCommand);
}

void AudioRecorder::setOutputLocation()
{
#ifdef Q_OS_ANDROID
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save Recording"),
        "output." + selectedMediaFormat().mimeType().preferredSuffix());
#else
    QString fileName = QFileDialog::getSaveFileName();
#endif
    m_recordingFacade->recorder()->setOutputLocation(QUrl::fromLocalFile(fileName));
    m_outputLocationSet = true;
}

void AudioRecorder::displayErrorMessage()
{
    ui->statusbar->showMessage(m_recordingFacade->errorString());
}

void AudioRecorder::onMediaFormatChanged()
{
    QMediaFormat format = m_recordingFacade->recorder()->mediaFormat();

    int formatIndex = ui->containerBox->findData(format.fileFormat());
    if (formatIndex != -1)
        ui->containerBox->setCurrentIndex(formatIndex);

    int codecIndex = ui->audioCodecBox->findData(QVariant::fromValue(format.audioCodec()));
    if (codecIndex != -1)
        ui->audioCodecBox->setCurrentIndex(codecIndex);
}

void AudioRecorder::updateDevices()
{
    const auto currentDevice = boxValue(ui->audioDeviceBox).value<QAudioDevice>();
    int currentDeviceIndex = 0;

    ui->audioDeviceBox->clear();

    ui->audioDeviceBox->addItem(tr("Default"), {});
    for (const auto &device : m_mediaDevices->audioInputs()) {
        const auto name = device.description();
        ui->audioDeviceBox->addItem(name, QVariant::fromValue(device));

        if (device.id() == currentDevice.id())
            currentDeviceIndex = ui->audioDeviceBox->count() - 1;
    }

    ui->audioDeviceBox->setCurrentIndex(currentDeviceIndex);
}

void AudioRecorder::updateFormats()
{
    if (m_updatingFormats)
        return;
    m_updatingFormats = true;

    QMediaFormat format;
    if (ui->containerBox->count())
        format.setFileFormat(boxValue(ui->containerBox).value<QMediaFormat::FileFormat>());

    // Add all supported formats, remembering the previous choice
    int currentIndex = 0;
    ui->containerBox->clear();
    ui->containerBox->addItem(tr("Default file format"),
                              QVariant::fromValue(QMediaFormat::UnspecifiedFormat));
    for (auto container : format.supportedFileFormats(QMediaFormat::Encode)) {
        if (container < QMediaFormat::Mpeg4Audio) // Skip video formats
            continue;
        if (container == format.fileFormat())
            currentIndex = ui->containerBox->count();
        ui->containerBox->addItem(QMediaFormat::fileFormatDescription(container),
                                  QVariant::fromValue(container));
    }
    ui->containerBox->setCurrentIndex(currentIndex);

    // Re-populate codecs, adding only those that the chosen file format supports.
    // Remember current codec if it is supported by the file format.
    QMediaFormat::AudioCodec currentCodec = QMediaFormat::AudioCodec::Unspecified;
    if (ui->audioCodecBox->count())
        currentCodec = boxValue(ui->audioCodecBox).value<QMediaFormat::AudioCodec>();

    currentIndex = 0;
    ui->audioCodecBox->clear();
    ui->audioCodecBox->addItem(tr("Default audio codec"),
                               QVariant::fromValue(QMediaFormat::AudioCodec::Unspecified));
    for (auto codec : format.supportedAudioCodecs(QMediaFormat::Encode)) {
        if (codec == currentCodec)
            currentIndex = ui->audioCodecBox->count();
        ui->audioCodecBox->addItem(QMediaFormat::audioCodecDescription(codec),
                                   QVariant::fromValue(codec));
    }
    ui->audioCodecBox->setCurrentIndex(currentIndex);

    m_updatingFormats = false;
}

void AudioRecorder::clearAudioLevels()
{
    for (auto m_audioLevel : std::as_const(m_audioLevels))
        m_audioLevel->setLevel(0);
}

QMediaFormat AudioRecorder::selectedMediaFormat() const
{
    QMediaFormat format;
    format.setFileFormat(boxValue(ui->containerBox).value<QMediaFormat::FileFormat>());
    format.setAudioCodec(boxValue(ui->audioCodecBox).value<QMediaFormat::AudioCodec>());
    return format;
}

// New method to display command history
void AudioRecorder::displayCommandHistory()
{
    // Find the history list widget in the dock
    QDockWidget* dock = findChild<QDockWidget*>();
    if (dock) {
        QListWidget* historyList = qobject_cast<QListWidget*>(dock->widget());
        if (historyList) {
            historyList->clear();
            QStringList history = m_commandInvoker->getCommandHistory();
            for (const QString& cmd : history) {
                historyList->addItem(cmd);
            }
        }
    }
}

// New slot to handle command execution
void AudioRecorder::onCommandExecuted(const QString& commandName)
{
    ui->statusbar->showMessage(tr("Command executed: %1").arg(commandName), 2000);
    displayCommandHistory();
}

// returns the audio level for each channel
QList<qreal> getBufferLevels(const QAudioBuffer &buffer)
{
    QList<qreal> values;

    auto format = buffer.format();
    if (!format.isValid())
        return values;

    int channels = buffer.format().channelCount();
    values.fill(0, channels);

    int bytesPerSample = format.bytesPerSample();
    QList<qreal> max_values;
    max_values.fill(0, channels);

    const char *data = buffer.constData<char>();
    for (int i = 0; i < buffer.frameCount(); ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(format.normalizedSampleValue(data));
            if (value > max_values.at(j))
                max_values[j] = value;
            data += bytesPerSample;
        }
    }

    return max_values;
}

void AudioRecorder::processBuffer(const QAudioBuffer &buffer)
{
    if (m_audioLevels.count() != buffer.format().channelCount()) {
        qDeleteAll(m_audioLevels);
        m_audioLevels.clear();
        for (int i = 0; i < buffer.format().channelCount(); ++i) {
            AudioLevel *level = new AudioLevel(ui->centralwidget);
            m_audioLevels.append(level);
            ui->levelsLayout->addWidget(level);
        }
    }

    QList<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i)
        m_audioLevels.at(i)->setLevel(levels.at(i));
}
