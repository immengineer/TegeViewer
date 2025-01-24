#ifndef MYIMAGESCENE_H
#define MYIMAGESCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QVector>
#include <QVideoFrame>
#include <QTimer>

#include "define.h"
#include "utility.h"

class MyImageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyImageScene(QWidget *parent = nullptr);

    void SetPixmap(QPixmap pixmap, bool *samesize);
    void SaveImage(QString filepath);

    double GetFitinRatio(int width, int height);
    void GetAnalysisData(analysisData *data);
    void GetAnalysisData(QRect rect, analysisData *data);
    bool CreateHistogramData();
    int GetHistogramData(int index, int color);
    int GetHistogramMaxNum();
    int GetPixelValue(int x, int y, int color);
    QVector<int> GetPixelValue(int x, int y);
    bool CreateGraphData(bool isHorizontal, bool average);
    void SetCursorColor(QColor color);
    QColor GetCursorColor();

    inline bool IsNullImage() {return currentImage.isNull();}
    inline int GetImageWidth() {return currentImage.width();}
    inline int GetImageHeight() {return currentImage.height();}
    inline bool IsInvalidROI() { return roiRect.width() == 0 && roiRect.height() == 0; }
    inline QRect GetImageRect() { return currentImage.rect();}

    int roiType;
    QRect roiRect;
    QRect fixedArea;

    int zoomPercent;
    int numOfGraphData;
    int numOfGraphSeries;
    QVector<int> vGraphX;
    QVector<QRgb> vGraphY;

    // UVCCamera
    void SetFrameData(QVideoFrame* frame);

signals:
    void signalPixelInfo(QString info);
    void signalUpdateRoi(QRect rect);

private slots:
    void changeRoiType(int type);
    void updateRoi();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    void drawCursor();
    void allocBuffer();
    void freeBuffer();

    QRect getSelectedArea();
    QPoint startPoint, endPoint;
    QImage currentImage;
    QColor cursorColor;
    QTimer *timer;

    int *pHistBuff[NUM_COLOR];

};

#endif // MYIMAGESCENE_H
