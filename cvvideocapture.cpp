#include "cvvideocapture.h"
QVector<QString> CAP_PROPERTY = {
    "MSEC",
    "POS_FRAMES",
    "POS_AVI_RATIO",
    "FRAME_WIDTH",
    "FRAME_HEIGHT",
    "PROP_FPS",
    "PROP_FOURCC",
    "FRAME_COUNT",
    "FORMAT",
    "MODE",
    "BRIGHTNESS",
    "CONTRAST",
    "SATURATION",
    "HUE",
    "GAIN",
    "EXPOSURE",
    "CONVERT_RGB",
    "WHITE_BALANCE",
    "RECTIFICATION",
    "MONOCHROME",
    "SHARPNESS",
    "AUTO_EXPOSURE",
    "GAMMA",
    "TEMPERATURE",
    "TRIGGER",
    "TRIGGER_DELAY",
    "WHITE_BALANCE_RED_V",
    "ZOOM",
    "FOCUS",
    "GUID",
    "ISO_SPEED",
    "DC1394_MAX",
    "BACKLIGHT",
    "PAN",
    "TILT",
    "ROLL",
    "IRIS",
    "SETTINGS",
    "BUFFERSIZE",
    "AUTOFOCUS",
};
CvVideoCapture::CvVideoCapture(QObject *parent) : QThread(parent)
{
    StopVideoCapture = false;
    capture = cv::VideoCapture();
}
CvVideoCapture::~CvVideoCapture()
{
    StopVideoCapture = true;
}
QVector<int> CvVideoCapture::InitOpenCVCaptute()
{
    QVector<int> vResult;

    if(capture.isOpened())
        capture.release();

    videoCaptureCount = 0;
    for (int i = 0; i < MAX_CAPTURE; ++i) {
        bool result = capture.open(i, cv::CAP_ANY);
        if (result) {
            videoCaptureCount++;
            vResult.append(i);
        }
    }
    return vResult;
}
bool CvVideoCapture::OpenVideoCapture(int index)
{
    if (capture.isOpened())
        capture.release();

    return capture.open(index, cv::CAP_ANY);;
}

double CvVideoCapture::GetCaptureProperty(int id)
{
    return capture.get(id);
}

bool CvVideoCapture::SetCaptureProperty(int id, double value)
{
    return capture.set(id, value);
}
QString CvVideoCapture::GetAllProperty()
{
    QString propText = "";
    if (capture.isOpened()) {
        for (int i = 0; i < CAP_PROPERTY.size(); ++i)
            propText += CAP_PROPERTY[i] + " : " + QString::number(capture.get(i)) + "\n";
    }
    propText = propText.trimmed();
    return propText;
}

bool CvVideoCapture::SetCodec(QString format)
{
    std::string sVal = format.toStdString();
    const char* pCodec = sVal.c_str();
    bool result = capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc(pCodec[0], pCodec[1] , pCodec[2], pCodec[3]));

    return result;
}
void CvVideoCapture::GetMat(cv::Mat *img)
{
    *img = dst.clone();
}
void CvVideoCapture::DisableVideoCapture()
{
    if (capture.isOpened())
        capture.release();
}
void CvVideoCapture::run()
{
    QMutex mutex;
    qDebug() << "startCapture";
    qDebug() << "VideoCaptureMode:" << capture.get(cv::CAP_PROP_MODE);
    qDebug() << "FPS:" << capture.get(cv::CAP_PROP_FPS);
    while(true) {
        mutex.lock();
        if (this->StopVideoCapture) {
            qDebug() << "Stop Acpture!!";
            break;
        }
        mutex.unlock();
        capture >> frame;
        cv::cvtColor(frame, dst, cv::COLOR_BGR2RGB);
        emit updateCapture();
        g_pUtil->CalcFrameRate();
    }
    qDebug() << "FPS:" << capture.get(cv::CAP_PROP_FPS);
}


