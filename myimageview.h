#ifndef MYIMAGEVIEW_H
#define MYIMAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>

class MyImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyImageView(QWidget *parent = nullptr);
    void InitZoomLevel();
    int32_t zoomLimitMax;
    int32_t zoomLimitMin;

    inline qreal GetZoomRatio() { return scaleValue();}

signals:
    void signalDroppedURLs(QList<QUrl> urllist);
    void signalZoomRatio(qreal ratio);

public slots:
    void zoomOriginal();
    void zoomIn(int level = 1);
    void zoomOut(int level = 1);
    void fitWindow(double fRatio);

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

    using QGraphicsView::QGraphicsView;

private:
    int m_zoom_level;
    qreal scaleValue() const;
    void setMatrix();

    bool _pan;
    int _panStartX, _panStartY;

};

#endif // MYIMAGEVIEW_H
