#include "uvccamera.h"

UVCCamera::UVCCamera(QObject *parent)
{
    camera = nullptr;
}

bool UVCCamera::InitQCamera()
{
    camera = nullptr;
    sink = nullptr;
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.size() ==  0)
        return false;
    camera = new QCamera(cameras.at(0));
    if (!camera->isAvailable())
        return false;

    CameraDescription = cameras.at(0).description();
    captureSession.setCamera(camera);
    sink = new QVideoSink;
    captureSession.setVideoSink(sink);
    connect(sink, &QVideoSink::videoFrameChanged, this, &UVCCamera::getFrame);
    return true;
}

void UVCCamera::StartQCamera()
{
    camera->start();
    qDebug() << "startCamera";
}

void UVCCamera::StopQCamera()
{
    camera->stop();
    qDebug() << "stopCamera";
}
void UVCCamera::getFrame()
{
    QVideoFrame frame = captureSession.videoSink()->videoFrame();
    if (frame.isValid()) {
        emit newFrame(&frame);
        g_pUtil->CalcFrameRate();
    }
}
