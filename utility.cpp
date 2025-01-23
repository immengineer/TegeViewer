#include "utility.h"

Utility *g_pUtil;

Utility::Utility()
{
    frameCount = 0;
    frameRate = 0.0;
    dispCount = 0;
    dispRate = 0.0;
}

void Utility::InitTimer()
{
    frameCount = 0;
    frameRate = 0.0;
    timerFrameRate.start();
    dispCount = 0;
    dispRate = 0.0;
    timerDispRate.start();
}
void Utility::CalcFrameRate()
{
    frameCount++;
    double elapsed = (double)timerFrameRate.elapsed();
    if (elapsed > 1000.0) {
        frameRate = (double)frameCount / (elapsed/1000.0);
        frameCount = 0;
        timerFrameRate.restart();
    }
}
void Utility::CalcDispRate()
{
    dispCount++;
    double elapsed = (double)timerDispRate.elapsed();
    if (elapsed > 1000.0) {
        dispRate = (double)dispCount / (elapsed/1000.0);
        dispCount = 0;
        timerDispRate.restart();
    }
}
