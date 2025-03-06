#ifndef CVVIDEOCAPTURE_H
#define CVVIDEOCAPTURE_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QVector>
#include <QMutex>
#include <QChar>
#include "utility.h"
#include "define.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include "string.h"

class CvVideoCapture : public QThread
{
    Q_OBJECT
public:
    CvVideoCapture(QObject *parent = nullptr);
    ~CvVideoCapture();

    QVector<int> InitOpenCVCaptute();
    bool OpenVideoCapture(int index = 0);

    double GetCaptureProperty(int id);
    bool SetCaptureProperty(int id, double value);
    QString GetAllProperty();
    bool SetCodec(QString format);

    void GetMat(cv::Mat *img);
    void DisableVideoCapture();

    inline bool IsVideoCaptureReady() { return capture.isOpened();}
    volatile bool StopVideoCapture;

signals:
    void updateCapture();

    // QThread interface
protected:
    virtual void run();


private:
    cv::VideoCapture capture;
    cv::Mat frame, dst;
    int videoCaptureCount;
};

#endif // CVVIDEOCAPTURE_H
