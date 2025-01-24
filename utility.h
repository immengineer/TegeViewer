#ifndef UTILITY_H
#define UTILITY_H

#include <QTimer>
#include <QElapsedTimer>


class Utility
{
public:
    Utility();

    void InitTimer();
    void CalcFrameRate();
    void CalcDispRate();
    void SetCameraStatus(bool start);

    inline double GetFrameRateAvrg() {return frameRate;}
    inline double GetDispRateAvrg() {return dispRate;}
    inline bool IsCameraStarted() {return isCameraStarted;}

private:
    QElapsedTimer timerFrameRate;
    QElapsedTimer timerDispRate;
    qint64 frameCount;
    qint64 dispCount;
    double frameRate;
    double dispRate;

    bool isCameraStarted;

};
extern Utility *g_pUtil;
#endif // UTILITY_H
