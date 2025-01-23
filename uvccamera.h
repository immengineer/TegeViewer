#ifndef UVCCAMERA_H
#define UVCCAMERA_H

#include <QObject>
#include <QThread>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include "utility.h"

class UVCCamera : public QThread
{
    Q_OBJECT
public:
    explicit UVCCamera(QObject *parent = nullptr);
    bool InitQCamera();
    QString CameraDescription;
    void StartQCamera();
    void StopQCamera();

private slots:
    void getFrame();

signals:
    void newFrame(QVideoFrame* frame);

private:
    QCamera* camera;
    QMediaCaptureSession captureSession;
    QVideoSink* sink;
};

#endif // UVCCAMERA_H
