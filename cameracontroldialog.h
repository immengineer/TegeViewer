#ifndef CAMERACONTROLDIALOG_H
#define CAMERACONTROLDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QList>
#include <QListWidget>
#include <QtMultimedia>
#include <QVideoFrameFormat>
#include <QMap>
#include <QDebug>
#include <QWidget>
#include <QObject>
#include <QSlider>
#include "utility.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

namespace Ui {
class CameraControlDialog;
}

class CameraControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CameraControlDialog(QWidget *parent = nullptr);
    ~CameraControlDialog();

    inline bool IsShown(){return isShown;}
    QString CameraDescription;
    void StartQCamera();
    void StopQCamera();

private slots:
    void getFrame();
    void detectQCamera();
    void changeFormat();
    void openFFMPEGSettings();
    bool openCamera(int index = 0);
    void changeSetting();
    void initOpenCVCaptute();

signals:
    void cameraIsReady();
    void newFrame(QVideoFrame* frame);

private:
    Ui::CameraControlDialog *ui;
    void createActions();
    bool isShown;

    void initControls();
    void initializeSettingsGroup();

    QList<QCameraDevice> cameraDevices;
    QList<QCameraFormat> cameraFormats;
    QList<QVideoFrameFormat> videoFrameFormats;
    QCamera* camera;
    QMediaCaptureSession captureSession;
    QVideoSink* sink;

    cv::VideoCapture capture;
    // cv::Mat frame, dst;

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);
};

#endif // CAMERACONTROLDIALOG_H
