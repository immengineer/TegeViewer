#ifndef CAMERACONTROLDIALOG_H
#define CAMERACONTROLDIALOG_H

#include <QDialog>
#include <QWidget>
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
#include <QColor>
#include <QScrollBar>
#include <QThread>
#include <QMutex>
#include "utility.h"
#include "define.h"
#include "cvvideocapture.h"

namespace Ui {
class CameraControlDialog;
}

class CameraControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CameraControlDialog(QWidget *parent = nullptr, CvVideoCapture *capture = nullptr);
    ~CameraControlDialog();

    inline bool IsShown()   { return isShown; }
    inline bool IsQCameraReady() { return isQCameraReady; }
    QString CameraDescription;

    // QCamera
    void StartQCamera();
    void StopQCamera();

private slots:
    void changeSetting();

    // QCamera
    void getFrame();
    void detectQCamera();
    void changeFormat();
    void openFFMPEGSettings();
    bool openCamera(int index = 0);

    // OpenCV
    void initOpenCVCaptute();
    void openVideoCapture(int index = 0);
    void setLineEditorValue();
    void dispCaptureProperty();
    void openCaptureControl();
    void setFormat();

signals:
    void cameraStatus(bool IsReady);
    void newFrame(QVideoFrame* frame);

private:
    Ui::CameraControlDialog *ui;
    void createActions();
    bool isShown;

    // QCameraControl
    void initControls();
    void initializeSettingsGroup();
    void disableQCamera();

    QList<QCameraDevice> cameraDevices;
    QList<QCameraFormat> cameraFormats;
    QList<QVideoFrameFormat> videoFrameFormats;
    QCamera* camera;
    QMediaCaptureSession captureSession;
    QVideoSink* sink;
    volatile bool isQCameraReady;

    // OpenCV
    CvVideoCapture *videoCap;
    int videoCaptureCount;
    int captureWidth, captureHeight;
    double captureFps;

    void dispLineEdit();
    void addBrowserText(QString text);
    void addBrowserText(QColor color, QString text);
    void addBrowserText(bool bVal, QString text);
    void scrollBrowser();
    void disableVideoCapture();

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);
};

#endif // CAMERACONTROLDIALOG_H
