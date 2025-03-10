#include "cameracontroldialog.h"
#include "ui_cameracontroldialog.h"

QMap<QVideoFrameFormat::ColorSpace, QString> colorSpaceMap{
    {QVideoFrameFormat::ColorSpace_Undefined,       "Undefined"},
    {QVideoFrameFormat::ColorSpace_BT601,           "BT601"},
    {QVideoFrameFormat::ColorSpace_BT709,           "BT709"},
    {QVideoFrameFormat::ColorSpace_AdobeRgb,        "AdobeRGB"},
    {QVideoFrameFormat::ColorSpace_BT2020,          "BT2020"},
};
QMap<QVideoFrameFormat::ColorTransfer, QString> colorTransferMap{
    {QVideoFrameFormat::ColorTransfer_Unknown,      "UnKnown"},
    {QVideoFrameFormat::ColorTransfer_BT709,        "BT709"},
    {QVideoFrameFormat::ColorTransfer_BT601,        "BT601"},
    {QVideoFrameFormat::ColorTransfer_Linear,       "Linear"},
    {QVideoFrameFormat::ColorTransfer_Gamma22,      "Gamma22"},
    {QVideoFrameFormat::ColorTransfer_Gamma28,      "Gamma28"},
    {QVideoFrameFormat::ColorTransfer_ST2084,       "ST2084"},
    {QVideoFrameFormat::ColorTransfer_STD_B67,      "STD_B67"},
};
QMap<QVideoFrameFormat::Direction, QString> directionMap{
    {QVideoFrameFormat::TopToBottom,                "TopToBottom"},
    {QVideoFrameFormat::BottomToTop,                "BottomToTop"},
};
QMap<QVideoFrameFormat::PixelFormat, QString> pixelFormatMap{
    {QVideoFrameFormat::Format_Invalid,                 "Invalid" },
    {QVideoFrameFormat::Format_ARGB8888,                "ARGB8888" },
    {QVideoFrameFormat::Format_ARGB8888_Premultiplied,  "ARGB8888_Premultiplied" },
    {QVideoFrameFormat::Format_XRGB8888,                "RGB8888" },
    {QVideoFrameFormat::Format_BGRA8888,                "BGRA8888" },
    {QVideoFrameFormat::Format_BGRA8888_Premultiplied,  "BGRA8888_Premultiplied" },
    {QVideoFrameFormat::Format_ABGR8888,                "ABGR8888" },
    {QVideoFrameFormat::Format_ABGR8888,                "ABGR8888_Premultiplied" },
    {QVideoFrameFormat::Format_XBGR8888,                "XBGR8888" },
    {QVideoFrameFormat::Format_RGBA8888,                "RGBA8888" },
    {QVideoFrameFormat::Format_BGRX8888,                "BGRX8888" },
    {QVideoFrameFormat::Format_RGBX8888,                "RGBX8888" },
    {QVideoFrameFormat::Format_AYUV,                    "AYUV" },
    {QVideoFrameFormat::Format_AYUV_Premultiplied,      "AYUV_Premultiplied" },
    {QVideoFrameFormat::Format_YUV420P,                 "YUV420P" },
    {QVideoFrameFormat::Format_YUV422P,                 "YUV422P" },
    {QVideoFrameFormat::Format_YV12,                    "YV12" },
    {QVideoFrameFormat::Format_UYVY,                    "UYVY" },
    {QVideoFrameFormat::Format_YUYV,                    "YUYV" },
    {QVideoFrameFormat::Format_NV12,                    "NV12" },
    {QVideoFrameFormat::Format_NV21,                    "NV21" },
    {QVideoFrameFormat::Format_IMC1,                    "IMC1" },
    {QVideoFrameFormat::Format_IMC2,                    "IMC2" },
    {QVideoFrameFormat::Format_IMC3,                    "IMC3" },
    {QVideoFrameFormat::Format_IMC4,                    "IMC4" },
    {QVideoFrameFormat::Format_P010,                    "P010" },
    {QVideoFrameFormat::Format_P016,                    "P016" },
    {QVideoFrameFormat::Format_Y8,                      "Y8" },
    {QVideoFrameFormat::Format_Y16,                     "Y16" },
    {QVideoFrameFormat::Format_Jpeg,                    "Jpeg" },
    {QVideoFrameFormat::Format_SamplerExternalOES,      "SamplerExternalOES" },
    {QVideoFrameFormat::Format_SamplerRect,             "SamplerRect" },
    {QVideoFrameFormat::Format_YUV420P10,               "YUV420P10" },
};
QMap<QString, QCamera::ExposureMode> exposureModeMap{
    {"Auto",            QCamera::ExposureMode::ExposureAuto },
    {"Manual",          QCamera::ExposureMode::ExposureManual },
    {"Portrait",        QCamera::ExposureMode::ExposurePortrait },
    {"Night",           QCamera::ExposureMode::ExposureNight },
    {"Sports",          QCamera::ExposureMode::ExposureSports },
    {"Snow",            QCamera::ExposureMode::ExposureSnow },
    {"Beach",           QCamera::ExposureMode::ExposureBeach },
    {"Action",          QCamera::ExposureMode::ExposureAction },
    {"Landscape",       QCamera::ExposureMode::ExposureLandscape },
    {"Night Portrait",  QCamera::ExposureMode::ExposureNightPortrait },
    {"Theatre",         QCamera::ExposureMode::ExposureTheatre },
    {"Sunset",          QCamera::ExposureMode::ExposureSunset },
    {"Steady Photo",    QCamera::ExposureMode::ExposureSteadyPhoto },
    {"Fireworks",       QCamera::ExposureMode::ExposureFireworks },
    {"Party",           QCamera::ExposureMode::ExposureParty },
    {"Candlelight",     QCamera::ExposureMode::ExposureCandlelight },
    {"Barcode",         QCamera::ExposureMode::ExposureBarcode }
};
QMap<QString, QCamera::FlashMode> flashModeMap{
    {"Off",             QCamera::FlashMode::FlashOff },
    {"On",              QCamera::FlashMode::FlashOn },
    {"Auto",            QCamera::FlashMode::FlashAuto }
};
QMap<QString, QCamera::TorchMode> torchModeMap{
    {"Off",             QCamera::TorchMode::TorchOff },
    {"On",              QCamera::TorchMode::TorchOn },
    {"Auto",            QCamera::TorchMode::TorchAuto }
};
QMap<QString, QCamera::WhiteBalanceMode> whiteBalanceModeMap{
    {"Auto",            QCamera::WhiteBalanceMode::WhiteBalanceAuto },
    {"Manual",          QCamera::WhiteBalanceMode::WhiteBalanceManual },
    {"Sunlight",        QCamera::WhiteBalanceMode::WhiteBalanceSunlight },
    {"Cloudy",          QCamera::WhiteBalanceMode::WhiteBalanceCloudy },
    {"Shade",           QCamera::WhiteBalanceMode::WhiteBalanceShade },
    {"Tungsten",        QCamera::WhiteBalanceMode::WhiteBalanceTungsten },
    {"Fluorescent",     QCamera::WhiteBalanceMode::WhiteBalanceFluorescent },
    {"Flash",           QCamera::WhiteBalanceMode::WhiteBalanceFlash },
    {"Sunset",          QCamera::WhiteBalanceMode::WhiteBalanceSunset }
};
QMap<QString, QCamera::FocusMode> focusModeMap{
    {"Auto",            QCamera::FocusMode::FocusModeAuto },
    {"Auto (Near)",     QCamera::FocusMode::FocusModeAutoNear },
    {"Auto (Far)",      QCamera::FocusMode::FocusModeAutoFar },
    {"Hyperfocal",      QCamera::FocusMode::FocusModeHyperfocal },
    {"Infinity",        QCamera::FocusMode::FocusModeInfinity },
    {"Manual",          QCamera::FocusMode::FocusModeManual }
};
CameraControlDialog::CameraControlDialog(QWidget *parent, CvVideoCapture *capture)
    : QDialog(parent)
    , ui(new Ui::CameraControlDialog)
    , isShown(false)
    , isQCameraReady(false)
{
    videoCap = capture;
    ui->setupUi(this);
    ui->tabWidget->setEnabled(false);
    ui->groupBoxCaptureSetting->setEnabled(false);
    setFixedSize(300, 560);
    createActions();
}

CameraControlDialog::~CameraControlDialog()
{
    delete ui;
}
void CameraControlDialog::StartQCamera()
{
    camera->start();
    ui->pushButtonDetectCamera->setEnabled(false);
    ui->comboVideoDevices->setEnabled(false);
    ui->comboBoxFormats->setEnabled(false);
}
void CameraControlDialog::StopQCamera()
{
    camera->stop();
    ui->comboVideoDevices->setEnabled(true);
    ui->comboBoxFormats->setEnabled(true);
    ui->pushButtonDetectCamera->setEnabled(true);
}

void CameraControlDialog::changeSetting()
{

}
void CameraControlDialog::getFrame()
{
    QVideoFrame frame = captureSession.videoSink()->videoFrame();
    if (frame.isValid()) {
        emit newFrame(&frame);
        g_pUtil->CalcFrameRate();
    }
}
void CameraControlDialog::detectQCamera()
{
    disableVideoCapture();

    ui->comboVideoDevices->blockSignals(true);
    ui->comboVideoDevices->clear();
    cameraDevices.clear();
    cameraDevices = QMediaDevices::videoInputs();

    if (cameraDevices.size() == 0) {
        QMessageBox::warning(this, "Error", "No available camera found.");
        return;
    }
    for (QCameraDevice& cd : cameraDevices) {
        ui->comboVideoDevices->addItem(cd.description());
    }

    // open firstcamera as default
    openCamera();

    ui->comboVideoDevices->blockSignals(false);
}

void CameraControlDialog::changeFormat()
{
    int index = ui->comboBoxFormats->currentIndex();
    if (index < 0)
        return;
    else if (index < cameraFormats.size())
        camera->setCameraFormat(cameraFormats.at(index));
}

void CameraControlDialog::openFFMPEGSettings()
{
// Check OS for compatibility
#ifdef Q_OS_WIN
#else
    QMessageBox::warning(this, "Error", "FFMPEG settings are only available on Windows.");
    return;
#endif

    QProcess* process = new QProcess();

    // Handle errors
    connect(process, &QProcess::errorOccurred, [this](QProcess::ProcessError error) {
        switch (error) {
        case QProcess::ProcessError::FailedToStart:
            QMessageBox::warning(this, "Error", "Failed to start ffmpeg process.");
            break;
        case QProcess::ProcessError::Crashed:
            QMessageBox::warning(this, "Error", "FFMPEG process crashed.");
            break;
        case QProcess::ProcessError::Timedout:
            QMessageBox::warning(this, "Error", "FFMPEG process timed out.");
            break;
        case QProcess::ProcessError::WriteError:
            QMessageBox::warning(this, "Error", "FFMPEG process write error.");
            break;
        case QProcess::ProcessError::ReadError:
            QMessageBox::warning(this, "Error", "FFMPEG process read error.");
            break;
        case QProcess::ProcessError::UnknownError:
            QMessageBox::warning(this, "Error", "FFMPEG process unknown error.");
            break;
        default:
            QMessageBox::warning(this, "Error", "FFMPEG process error.");
            break;
        }
    });
    QStringList args;
    args << "-f" << "dshow" << "-show_video_device_dialog" << "true" << "-i" << "video=" + this->CameraDescription;
    process->start("ffmpeg", args);
}
bool CameraControlDialog::openCamera(int index /* = 0 */)
{
    ui->tabWidget->setDisabled(true);
    g_pUtil->SetCameraStatus(false);
    camera = nullptr;
    sink = nullptr;
    CameraDescription = "";
    isQCameraReady = false;
    camera = new QCamera(cameraDevices.at(index)); 

    if (!camera->isAvailable()) {
        QMessageBox::warning(this, "Warning", "Failed to open QCamera : " + cameraDevices.at(index).description());
        return false;
    }

    // QCamera::Features features = camera->supportedFeatures();
    // bool bVal = features& QCamera::Feature::ColorTemperature;
    // qDebug() << "ColorTemperature = " <<  bVal;
    // bVal = features& QCamera::Feature::ExposureCompensation;
    // qDebug() << "ExposureCompensation = " <<  bVal;
    // bVal = features& QCamera::Feature::IsoSensitivity;
    // qDebug() << "IsoSensitivity = " <<  bVal;
    // bVal = features& QCamera::Feature::ManualExposureTime;
    // qDebug() << "ManualExposureTime = " <<  bVal;
    // bVal = features& QCamera::Feature::CustomFocusPoint;
    // qDebug() << "CustomFocusPoint = " <<  bVal;
    // bVal = features& QCamera::Feature::FocusDistance;
    // qDebug() << "FocusDistance = " <<  bVal;

    cameraFormats.clear();
    cameraFormats = camera->cameraDevice().videoFormats();
    int count = cameraFormats.size();
    if (count == 0) {
        ui->comboBoxFormats->hide();
        return false;
    }
    ui->comboBoxFormats->clear();
    for (int i = 0; i < count; ++i) {
        QCameraFormat cmf = cameraFormats.at(i);

        ui->comboBoxFormats->addItem(QString::number(cmf.resolution().width()) + " x " +
                                     QString::number(cmf.resolution().height()) + " : " +
                                     pixelFormatMap[cmf.pixelFormat()] + "  " +
                                     QString::number(cmf.maxFrameRate()) + "FPS");
    }

    camera->setCameraFormat(cameraFormats.at(0));
    CameraDescription = cameraDevices.at(index).description();
    captureSession.setCamera(camera);
    sink = new QVideoSink;
    captureSession.setVideoSink(sink);
    connect(sink, &QVideoSink::videoFrameChanged, this, &CameraControlDialog::getFrame);
    initControls();
    initializeSettingsGroup();
    isQCameraReady = true;
    emit cameraStatus(true);

    ui->tabWidget->setEnabled(true);
    return true;
}
void CameraControlDialog::initOpenCVCaptute()
{
    this->setCursor(Qt::WaitCursor);
    if (camera != nullptr) {
        disableQCamera();
    }
    ui->comboVideoCapture->blockSignals(true);
    videoCaptureCount = 0;
    ui->comboVideoCapture->clear();

    QVector<int> vIndex = videoCap->InitOpenCVCaptute();
    videoCaptureCount = vIndex.size();
    for (int i = 0; i < videoCaptureCount; ++i) {
        int index = vIndex[i];
        ui->comboVideoCapture->addItem(QString::number(index));
    }
    ui->comboVideoCapture->blockSignals(false);
    addBrowserColorText(Qt::blue, "Number of VideoCapture >>> " + QString::number(videoCaptureCount));
    if (videoCaptureCount == 0) {
        QMessageBox::warning(this, "Warning", "No available capture found.");
        this->setCursor(Qt::ArrowCursor);
        return;
    }

    // Open 1st camera as default
    openVideoCapture();
}

void CameraControlDialog::openVideoCapture(int index)
{
    this->setCursor(Qt::WaitCursor);
    g_pUtil->SetCameraStatus(false);
    bool result = videoCap->OpenVideoCapture(index);
    if (result) {
        CameraDescription = "cv::VideoCapture(" + QString::number(index) + ")";
        addBrowserColorText(Qt::blue, "Open::" + CameraDescription);
        ui->groupBoxCaptureSetting->setEnabled(true);
        emit cameraStatus(true);
        this->setCursor(Qt::ArrowCursor);
    }
    else {
        this->setCursor(Qt::ArrowCursor);
        QMessageBox::warning(this, "Warning", "Failed to open capture index=" + QString::number(index));
    }

}

void CameraControlDialog::setLineEditorValue()
{
    if (videoCap->IsVideoCaptureReady()) {
        int iVal;
        double dVal;
        bool result;
        QString msg = "";
        iVal = ui->lineEditWidth->text().toInt();
        if (iVal != captureWidth) {
            result = videoCap->SetCaptureProperty(cv::CAP_PROP_FRAME_WIDTH, iVal);
            msg = "Width : " + QString::number(captureWidth) + " ==> " + QString::number(iVal);
            addBrowserText(result, msg);
        }
        iVal =  ui->lineEditHeight->text().toInt();
        if (iVal != captureHeight) {
            result = videoCap->SetCaptureProperty(cv::CAP_PROP_FRAME_HEIGHT, iVal);
            msg = "Height : " + QString::number(captureHeight) + " ==> " + QString::number(iVal);
            addBrowserText(result, msg);
        }
        dVal = ui->lineEditFps->text().toDouble();
        if (dVal != captureFps) {
            qDebug() << "captureFps=" + QString::number(captureFps, 'f', 5);
            qDebug() << "dVal=" + QString::number(dVal, 'f', 5);
            result = videoCap->SetCaptureProperty(cv::CAP_PROP_FPS, dVal);
            msg = "Fps : " + QString::number(captureFps) + " ==> " + QString::number(dVal);
            addBrowserText(result, msg);
        }
    }
}

void CameraControlDialog::dispCaptureProperty()
{
    QString propText = videoCap->GetAllProperty();
    addBrowserText(propText);
    scrollBrowser();
    dispLineEdit();
}

void CameraControlDialog::openCaptureControl()
{
    videoCap->SetCaptureProperty(cv::CAP_PROP_SETTINGS, 1);
}

void CameraControlDialog::setFormat()
{
    QString format = ui->lineEditFormat->text();
    format = format.toUpper();
    if (format.length() == 4) {
        bool result = videoCap->SetCodec(format);
        addBrowserText(result, "Set VideoFormat :" + format);
    }
}
void CameraControlDialog::createActions()
{
    connect(ui->pushButtonDetectCamera, &QPushButton::clicked, this, &CameraControlDialog::detectQCamera);
    connect(ui->comboBoxFormats, &QComboBox::currentIndexChanged, this, &CameraControlDialog::changeFormat);
    connect(ui->comboVideoDevices, SIGNAL(currentIndexChanged(int)), this, SLOT(openCamera(int)));
    connect(ui->tabWidgetControl, &QTabWidget::currentChanged, this, &CameraControlDialog::changeSetting);

    connect(ui->pushButtonInitOpenCVCapture, &QPushButton::clicked, this, &CameraControlDialog::initOpenCVCaptute);
    connect(ui->comboVideoCapture, SIGNAL(currentIndexChanged(int)), this, SLOT(openVideoCapture(int)));
    connect(ui->pushButtonClear, &QPushButton::clicked, [this]() { ui->textBrowser->clear(); });

    connect(ui->lineEditWidth, SIGNAL(editingFinished()), this, SLOT(setLineEditorValue()));
    connect(ui->lineEditHeight, SIGNAL(editingFinished()), this, SLOT(setLineEditorValue()));
    connect(ui->lineEditFps, SIGNAL(editingFinished()), this, SLOT(setLineEditorValue()));
    connect(ui->pushButtonGetParam, &QPushButton::clicked, this, &CameraControlDialog::dispCaptureProperty);
    connect(ui->pushButtonControl,  &QPushButton::clicked, this, &CameraControlDialog::openCaptureControl);
    connect(ui->lineEditFormat, SIGNAL(editingFinished()), this, SLOT(setFormat()));
}
void CameraControlDialog::initControls()
{
    ui->tabWidget->setEnabled(false);

    if (!camera->isAvailable())
        return;

    QCamera::Features features = camera->supportedFeatures();

    bool bEnable;

    bEnable = features & QCamera::Feature::ColorTemperature;
    ui->sliderColorTemp->setEnabled(bEnable);
    ui->labelColorTemperature->setEnabled(bEnable);

    bEnable = features & QCamera::Feature::ExposureCompensation;
    ui->sliderExposureComp->setEnabled(bEnable);
    ui->labelExposureComp->setEnabled(bEnable);

    bEnable = features & QCamera::Feature::IsoSensitivity;
    ui->sliderManualIsoSensitivity->setEnabled(bEnable);
    ui->labelManualIsoSensitivity->setEnabled(bEnable);
    ui->checkboxAutoIsoSensitivity->setEnabled(bEnable);
    ui->labelAutoIsoSensitivity->setEnabled(bEnable);

    bEnable = features & QCamera::Feature::ManualExposureTime;
    ui->sliderManualExposureTime->setEnabled(bEnable);
    ui->labelManualExposureTime->setEnabled(bEnable);
    ui->checkboxAutoExposureTime->setEnabled(bEnable);
    ui->labelAutoExposureTime->setEnabled(bEnable);

    bEnable = features & QCamera::Feature::FocusDistance;
    ui->sliderFocusDistance->setEnabled(bEnable);
    ui->labelFocusDistance->setEnabled(bEnable);

    // ExposureMode
    ui->dropdownExposureMode->blockSignals(true);
    ui->dropdownExposureMode->clear();
    for (auto itr = exposureModeMap.begin(); itr != exposureModeMap.end(); itr++)
    {
        QString name = itr.key();
        QCamera::ExposureMode mode = itr.value();

        // Add mode if it is supported
        if (camera->isExposureModeSupported(mode)) ui->dropdownExposureMode->addItem(name, mode);
    }
    ui->dropdownExposureMode->blockSignals(false);

    // FlashMode
    ui->dropdownFlashMode->blockSignals(true);
    ui->dropdownFlashMode->clear();
    for (auto itr = flashModeMap.begin(); itr != flashModeMap.end(); itr++)
    {
        QString name = itr.key();
        QCamera::FlashMode mode = itr.value();

        // Add mode if it is supported
        if (camera->isFlashModeSupported(mode)) ui->dropdownFlashMode->addItem(name, mode);
    }
    ui->dropdownFlashMode->blockSignals(false);

    // FocusMode
    ui->dropdownFocusMode->blockSignals(true);
    ui->dropdownFocusMode->clear();
    for (auto itr = focusModeMap.begin(); itr != focusModeMap.end(); itr++)
    {
        QString name = itr.key();
        QCamera::FocusMode mode = itr.value();

        // Add mode if it is supported
        if (camera->isFocusModeSupported(mode)) ui->dropdownFocusMode->addItem(name, mode);
    }
    ui->dropdownFocusMode->blockSignals(false);

    // TorchMode
    ui->dropdownTorchMode->blockSignals(true);
    ui->dropdownTorchMode->clear();
    for (auto itr = torchModeMap.begin(); itr != torchModeMap.end(); itr++)
    {
        QString name = itr.key();
        QCamera::TorchMode mode = itr.value();

        // Add mode if it is supported
        if (camera->isTorchModeSupported(mode)) ui->dropdownTorchMode->addItem(name, mode);
    }
    ui->dropdownTorchMode->blockSignals(false);

    // WhiteBalanceMode
    ui->dropdownWhiteBalanceMode->blockSignals(true);
    ui->dropdownWhiteBalanceMode->clear();
    for (auto itr = whiteBalanceModeMap.begin(); itr != whiteBalanceModeMap.end(); itr++)
    {
        QString name = itr.key();
        QCamera::WhiteBalanceMode mode = itr.value();

        // Add mode if it is supported
        if (camera->isWhiteBalanceModeSupported(mode)) ui->dropdownWhiteBalanceMode->addItem(name, mode);
    }
    ui->dropdownWhiteBalanceMode->blockSignals(false);
}

void CameraControlDialog::initializeSettingsGroup()
{
    /// Initialize Settings Box values
    // Controls
    ui->sliderColorTemp->setValue(camera->colorTemperature());
    ui->dropdownExposureMode->setCurrentIndex(static_cast<int>(camera->exposureMode()));
    ui->sliderExposureComp->setValue(camera->exposureCompensation());
    ui->dropdownFlashMode->setCurrentIndex(static_cast<int>(camera->flashMode()));
    ui->checkboxAutoExposureTime->setChecked(camera->exposureTime());
    ui->sliderManualExposureTime->setValue(camera->manualExposureTime());
    ui->checkboxAutoIsoSensitivity->setChecked(camera->isoSensitivity());
    ui->sliderManualIsoSensitivity->setValue(camera->manualIsoSensitivity());
    ui->dropdownTorchMode->setCurrentIndex(static_cast<int>(camera->torchMode()));
    ui->dropdownWhiteBalanceMode->setCurrentIndex(static_cast<int>(camera->whiteBalanceMode()));

    /// Initialize min/max values
    //ui->sliderColorTemp->setMinimum();
    //ui->sliderColorTemp->setMaximum();
    //ui->sliderExposureComp->setMinimum();
    //ui->sliderExposureComp->setMaximum();
    ui->sliderManualExposureTime->setMinimum(camera->minimumExposureTime());
    ui->sliderManualExposureTime->setMaximum(camera->maximumExposureTime());
    ui->sliderManualIsoSensitivity->setMinimum(camera->minimumIsoSensitivity());
    ui->sliderManualIsoSensitivity->setMaximum(camera->maximumIsoSensitivity());

    // Connect widgets to controls
    // Settings TAB
    connect(ui->sliderColorTemp, SIGNAL(valueChanged(int)), camera, SLOT(setColorTemperature(int)));

    connect(ui->dropdownExposureMode, &QComboBox::currentIndexChanged, camera, [this](int index) {
        camera->setExposureMode(static_cast<QCamera::ExposureMode>(index));
    });
    connect(ui->sliderExposureComp, &QSlider::valueChanged, camera, [this](int value) {
        camera->setExposureCompensation(value);
    });
    connect(ui->dropdownFlashMode, &QComboBox::currentIndexChanged, camera, [this](int index) {
        camera->setFlashMode(static_cast<QCamera::FlashMode>(index));
    });
    connect(ui->checkboxAutoExposureTime, &QCheckBox::checkStateChanged, camera, [this](Qt::CheckState state) {
        camera->setAutoExposureTime(); // TODO: add functionality
    });
    connect(ui->sliderManualExposureTime, &QSlider::valueChanged, camera, [this](int value) {
        camera->setManualExposureTime(value);
    });
    connect(ui->checkboxAutoIsoSensitivity, &QCheckBox::checkStateChanged, camera, [this](Qt::CheckState state) {
        camera->setAutoIsoSensitivity(); // TODO: add functionality
    });
    connect(ui->sliderManualIsoSensitivity, &QSlider::valueChanged, camera, [this](int value) {
        camera->setManualIsoSensitivity(value);
    });
    connect(ui->dropdownTorchMode, &QComboBox::currentIndexChanged, camera, [this](int index) {
        camera->setTorchMode(static_cast<QCamera::TorchMode>(index));
    });
    connect(ui->dropdownWhiteBalanceMode, &QComboBox::currentIndexChanged, camera, [this](int index) {
        camera->setWhiteBalanceMode(static_cast<QCamera::WhiteBalanceMode>(index));
    });

    // Zoom Focus TAB
    connect(ui->sliderZoomRate, &QSlider::valueChanged, camera, [this](int value) {
        camera->zoomTo(camera->zoomFactor(), value);
    });

    connect(ui->sliderZoom, &QSlider::valueChanged, camera, [this](int value) {
        camera->setZoomFactor(value);
        camera->zoomTo(value, 1);
    });

    connect(ui->dropdownFocusMode, &QComboBox::currentIndexChanged, camera, [this](int index) {
        camera->setFocusMode(static_cast<QCamera::FocusMode>(index));
    });

    connect(ui->sliderFocusDistance, &QSlider::valueChanged, camera, [this](int value) {
        camera->setFocusDistance(value);
    });
}

void CameraControlDialog::disableQCamera()
{
    camera = nullptr;
    sink = nullptr;
    isQCameraReady = false;
    ui->comboVideoDevices->blockSignals(true);
    ui->comboVideoDevices->clear();
    ui->comboVideoDevices->blockSignals(false);
    ui->comboBoxFormats->blockSignals(true);
    ui->comboBoxFormats->clear();
    ui->comboBoxFormats->blockSignals(false);

    ui->tabWidget->setEnabled(false);
    emit cameraStatus(false);
}
void CameraControlDialog::dispLineEdit()
{
    captureWidth = (int)videoCap->GetCaptureProperty(cv::CAP_PROP_FRAME_WIDTH);
    captureHeight = (int)videoCap->GetCaptureProperty(cv::CAP_PROP_FRAME_HEIGHT);

    double dVal = videoCap->GetCaptureProperty(cv::CAP_PROP_FPS);
    captureFps = std::round(dVal * 100) / 100;

    ui->lineEditWidth->setText(QString::number(captureWidth));
    ui->lineEditHeight->setText(QString::number(captureHeight));
    ui->lineEditFps->setText(QString::number(captureFps));
}

void CameraControlDialog::addBrowserText(QString text)
{
    ui->textBrowser->setTextColor(Qt::black);
    ui->textBrowser->insertPlainText(text + "\n");
}
void CameraControlDialog::addBrowserColorText(QColor color, QString text)
{
    ui->textBrowser->setTextColor(color);
    ui->textBrowser->insertPlainText(text + "\n");
}
void CameraControlDialog::addBrowserText(bool bVal, QString text)
{;
    QColor color = bVal ? Qt::black : Qt::red;
    ui->textBrowser->setTextColor(color);
    ui->textBrowser->insertPlainText(text + "\n");
}
void CameraControlDialog::scrollBrowser()
{
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void CameraControlDialog::disableVideoCapture()
{
    videoCap->DisableVideoCapture();

    ui->comboVideoCapture->blockSignals(true);
    ui->comboVideoCapture->clear();
    ui->comboVideoCapture->blockSignals(false);
    ui->groupBoxCaptureSetting->setEnabled(false);

    emit cameraStatus(false);
}

void CameraControlDialog::closeEvent(QCloseEvent *event)
{
    isShown = false;
}

void CameraControlDialog::showEvent(QShowEvent *event)
{
    isShown = true;
}

void CameraControlDialog::hideEvent(QHideEvent *event)
{
    isShown = false;
}
