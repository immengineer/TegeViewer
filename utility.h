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
    inline double GetFrameRateAvrg() {return frameRate;}
    inline double GetDispRateAvrg() {return dispRate;}

private:
    QElapsedTimer timerFrameRate;
    QElapsedTimer timerDispRate;
    qint64 frameCount;
    qint64 dispCount;
    double frameRate;
    double dispRate;

};
extern Utility *g_pUtil;
#endif // UTILITY_H
