#include "myimageview.h"

MyImageView::MyImageView(QWidget *parent) : QGraphicsView(parent)
{

}

void MyImageView::InitZoomLevel()
{
    m_zoom_level = 1;
}
void MyImageView::zoomOriginal()
{
    m_zoom_level = 0;
    setMatrix();
    emit signalZoomRatio(scaleValue());
}
void MyImageView::zoomIn(int level)
{
    if (m_zoom_level > zoomLimitMax)
        return;

    m_zoom_level += level;
    setMatrix();
    emit signalZoomRatio(scaleValue());
}

void MyImageView::zoomOut(int level)
{
    if (m_zoom_level < zoomLimitMin)
        return;

    m_zoom_level -= level;
    setMatrix();
    emit signalZoomRatio(scaleValue());
}

void MyImageView::fitWindow(double fRatio)
{
    QTransform mat;
    mat.scale(fRatio, fRatio);
    setTransform(mat);
    m_zoom_level = int(10.0 * std::log2(scaleValue()));
    emit signalZoomRatio(scaleValue());
}
void MyImageView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        const auto d = event->angleDelta();
        auto dm = abs(d.x()) > abs(d.y()) ? d.x() : d.y();
        if (dm > 0)
            zoomIn(3);
        else if (dm < 0)
            zoomOut(3);
        event->accept();
    }
    else
        QGraphicsView::wheelEvent(event);
}

void MyImageView::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton){
        _pan = true;
        auto pos = event->scenePosition();
        _panStartX = (int)pos.x();
        _panStartY = (int)pos.y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void MyImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        _pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MyImageView::mouseMoveEvent(QMouseEvent *event)
{
    if (_pan && event->buttons() == Qt::RightButton) {
        QPointF oldp(_panStartX, _panStartY);
        QPointF newp = event->scenePosition();
        QPointF translation = newp - oldp;
        setTransformationAnchor(QGraphicsView::NoAnchor);
        translate(translation.x(), translation.y());

        _panStartX = (int)newp.x();
        _panStartY = (int)newp.y();

        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}
void MyImageView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}
void MyImageView::dropEvent(QDropEvent *event)
{
    if (event->source() == this)
        return;

    if (event->mimeData()->hasUrls()){
        QList<QUrl> urllist = event->mimeData()->urls();
        emit signalDroppedURLs(urllist);
    }
}
qreal MyImageView::scaleValue() const
{
    auto square = [](qreal value) { return value * value; };
    return std::sqrt(square(transform().m11()) + square(transform().m12()));
}

void MyImageView::setMatrix()
{
    qreal newScale = std::pow(2.0, m_zoom_level / 10.0);
    QTransform mat;
    mat.scale(newScale, newScale);
    setTransform(mat);
}
