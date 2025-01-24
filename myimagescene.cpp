#include "myimagescene.h"

MyImageScene::MyImageScene(QWidget *parent) : QGraphicsScene(parent)
    , startPoint(-1, -1)
    , endPoint(-1, -1)
    , currentImage(nullptr)
    , cursorColor(Qt::gray)
    , numOfGraphData(0)
    , numOfGraphSeries(0)
{   
    zoomPercent = 100;
    allocBuffer();
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &MyImageScene::updateRoi);
    timer->start();
}

void MyImageScene::SetPixmap(QPixmap pixmap, bool *samesize)
{
    *samesize = false;
    if (IsNullImage()){
        startPoint.setX(-1);
        startPoint.setY(-1);
        endPoint.setX(-1);
        endPoint.setY(-1);
        freeBuffer();
        allocBuffer();
    }
    else {
        if (pixmap.size() == currentImage.size())
            *samesize = true;
        else {
            startPoint.setX(-1);
            startPoint.setY(-1);
            endPoint.setX(-1);
            endPoint.setY(-1);
            freeBuffer();
            allocBuffer();
        }
    }
    clear();
    addPixmap(pixmap);
    currentImage = pixmap.toImage();
    roiRect = getSelectedArea();
    drawCursor();
    emit signalUpdateRoi(roiRect);
}

void MyImageScene::SaveImage(QString filepath)
{
    currentImage.save(filepath);
}

void MyImageScene::SetFrameData(QVideoFrame* frame)
{
    if (IsNullImage() || (currentImage.size() != frame->size())){
        startPoint.setX(-1);
        startPoint.setY(-1);
        endPoint.setX(-1);
        endPoint.setY(-1);
        freeBuffer();
        allocBuffer();
        roiType = SELTYPE_RECT;
    }
    clear();
    currentImage = frame->toImage();
    QPixmap pixmap = QPixmap::fromImage(currentImage);
    addPixmap(pixmap);
    g_pUtil->CalcDispRate();
    roiRect = getSelectedArea();
    drawCursor();
}
double MyImageScene::GetFitinRatio(int width, int height)
{
    double fRatioX = (double)width / (double)currentImage.width();
    double fRatioY = (double)height / (double)currentImage.height();

    return std::min(fRatioX, fRatioY);
}

void MyImageScene::GetAnalysisData(analysisData *data)
{
    GetAnalysisData(roiRect, data);
}

void MyImageScene::GetAnalysisData(QRect rect, analysisData *data)
{
    int     x, y , color;
    double	fSum[NUM_COLOR];
    ulong	Sum[NUM_COLOR];
    int     pixelVal[NUM_COLOR];
    int		count;

    memset(data, 0, sizeof(analysisData));

    for (color = 0; color < NUM_COLOR; ++color){
        fSum[color] = 0.0;
        data->maxVal[color] = data->minVal[color] = 0;
        data->sn[color] = 0.0;
        Sum[color] = 0;
    }   
    // Sum , min, max
    count = 0;
    for (y = rect.top(); y <= rect.bottom(); ++y){
        for (x = rect.left(); x <= rect.right(); ++x){
            if (currentImage.valid(x, y)){
                QRgb rgb = currentImage.pixel(x, y);
                pixelVal[COLOR_INDEX_R] = qRed(rgb);
                pixelVal[COLOR_INDEX_G] = qGreen(rgb);
                pixelVal[COLOR_INDEX_B] = qBlue(rgb);

                for (color = 0; color < NUM_COLOR; ++color){
                    if (count == 0){
                        data->minValPos[color].setX(x);
                        data->minValPos[color].setY(y);
                        data->maxValPos[color].setX(x);
                        data->maxValPos[color].setY(y);
                        data->minVal[color] = data->maxVal[color] = pixelVal[color];
                    }
                    else {
                        if (pixelVal[color] < data->minVal[color]){
                            data->minVal[color] = pixelVal[color];
                            data->minValPos[color].setX(x);
                            data->minValPos[color].setY(y);
                        }
                        else if (pixelVal[color] > data->maxVal[color]){
                            data->maxVal[color] = pixelVal[color];
                            data->maxValPos[color].setX(x);
                            data->maxValPos[color].setY(y);
                        }
                    }
                    Sum[color] += pixelVal[color];
                }
                count++;
                data->pixels++;
            }
        }
    }
    // diff, average
    for (color = 0; color < NUM_COLOR; ++color){
        data->diffVal[color] = data->maxVal[color] - data->minVal[color];
        data->avrg[color] = (double)Sum[color] / (double)count;
    }

    // deviation
    for (y = rect.top(); y <= rect.bottom(); ++y){
        for (x = rect.left(); x <= rect.right(); ++x){
            if (currentImage.valid(x, y)){
                QRgb rgb = currentImage.pixel(x, y);
                pixelVal[COLOR_INDEX_R] = qRed(rgb);
                pixelVal[COLOR_INDEX_G] = qGreen(rgb);
                pixelVal[COLOR_INDEX_B] = qBlue(rgb);

                for (color = 0; color < NUM_COLOR; ++color){
                    double fVal = data->avrg[color] - pixelVal[color];
                    fSum[color] += (fVal * fVal);
                }
            }
        }
    }
    // standard deviation
    for (color = 0; color < NUM_COLOR; ++color){
        data->variance[color] = (double)fSum[color] / (double)count;
        data->sd[color] = sqrt(data->variance[color]);
        if (data->sd[color] != 0)
            data->sn[color] = 20.0 * log10((double)(HIST_NUM - 1) / data->sd[color]);
    }
}

bool MyImageScene::CreateHistogramData()
{
    memset(pHistBuff[COLOR_INDEX_R], 0, HIST_NUM * sizeof(int));
    memset(pHistBuff[COLOR_INDEX_G], 0, HIST_NUM * sizeof(int));
    memset(pHistBuff[COLOR_INDEX_B], 0, HIST_NUM * sizeof(int));

    if (IsNullImage() || IsInvalidROI())
        return false;

    for (int x = roiRect.left(); x <= roiRect.right(); ++x){
        for (int y = roiRect.top(); y <= roiRect.bottom(); ++y){
            QRgb rgb = currentImage.pixel(x, y);
            pHistBuff[COLOR_INDEX_R][qRed(rgb)]++;
            pHistBuff[COLOR_INDEX_G][qGreen(rgb)]++;
            pHistBuff[COLOR_INDEX_B][qBlue(rgb)]++;
        }
    }
    return true;
}

int MyImageScene::GetHistogramData(int index, int color)
{
    return pHistBuff[color][index];
}

int MyImageScene::GetHistogramMaxNum()
{
    int max = 0;
    for (int i = 0; i < HIST_NUM; ++i){
        for (int color = 0; color < NUM_COLOR; ++color){
            if (pHistBuff[color][i] > max)
                max = pHistBuff[color][i];
        }
    }
    return max;
}

int MyImageScene::GetPixelValue(int x, int y, int color)
{
    int iVal = -1;

    if (currentImage.valid(x, y)){
        QRgb rgb = currentImage.pixel(x, y);
        if (color == COLOR_INDEX_R)
            iVal = qRed(rgb);
        else if (color == COLOR_INDEX_G)
            iVal = qGreen(rgb);
        else if (color == COLOR_INDEX_B)
            iVal = qBlue(rgb);
    }
    return iVal;
}

QVector<int> MyImageScene::GetPixelValue(int x, int y)
{
    QVector<int> result;
    if (currentImage.valid(x, y)){
        QRgb rgb = currentImage.pixel(x, y);
        result.append(qRed(rgb));
        result.append(qGreen(rgb));
        result.append(qBlue(rgb));
    }
    return result;
}

bool MyImageScene::CreateGraphData(bool isHorizontal, bool average)
{
    int startPos = isHorizontal ? roiRect.left() : roiRect.top();
    int data = isHorizontal ? roiRect.width() : roiRect.height();
    int series = isHorizontal ? roiRect.height() : roiRect.width();

    int i, j, color, index;
    int posX = 0;
    int posY = 0;

    // clear data
    for (color = 0; color < NUM_COLOR; ++color)
        vGraphY.clear();

    vGraphX.clear();

    // X Axis
    for (i = 0; i < data; ++i)
        vGraphX.append(startPos + i);

    // Y Axis
    for (j = 0; j < series; ++j) {
        for (i = 0; i < data; ++i){
            posX = isHorizontal ? roiRect.left() + i : roiRect.left() + j;
            posY = isHorizontal ? roiRect.top() + j : roiRect.top() + i;

            if (currentImage.valid(posX, posY))
                vGraphY.append(currentImage.pixel(posX, posY));
        }
    }
    if (average){
        double sum[NUM_COLOR];
        uint8_t ave[NUM_COLOR];
        for (i = 0; i < data; ++i){
            for (color = 0; color < NUM_COLOR; ++color)
                sum[color] = 0.0;

            for (j = 0; j < series; ++j){
                index = (j * data) + i;
                QRgb rgb = vGraphY[index];
                sum[COLOR_INDEX_R] += qRed(rgb);
                sum[COLOR_INDEX_G] += qGreen(rgb);
                sum[COLOR_INDEX_B] += qBlue(rgb);
            }
            index = i;
            for (color = 0; color < NUM_COLOR; ++color){
                ave[color] = (uint8_t)(sum[color] / (double)series);
            }
            QRgb avergb = qRgb(ave[COLOR_INDEX_R], ave[COLOR_INDEX_G], ave[COLOR_INDEX_B]);
            vGraphY.replace(index, avergb);
        }
        numOfGraphSeries = 1;
    }
    else
        numOfGraphSeries = series;

    numOfGraphData = data;
    return true;
}

void MyImageScene::SetCursorColor(QColor color)
{
    cursorColor = color;
    drawCursor();
}

QColor MyImageScene::GetCursorColor()
{
    return cursorColor;
}
void MyImageScene::changeRoiType(int type)
{
    roiType = type;
    roiRect = getSelectedArea();
    drawCursor();
    emit signalUpdateRoi(roiRect);
}

void MyImageScene::updateRoi()
{
    if (g_pUtil->IsCameraStarted())
        emit signalUpdateRoi(roiRect);
}
void MyImageScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!IsNullImage()){
        int x = (int)event->scenePos().x();
        int y = (int)event->scenePos().y();
        QPoint currentPoint = QPoint(x, y);
        if (event->buttons() == Qt::LeftButton){
            if (x < 0 || x > currentImage.width() || y < 0 || y > currentImage.height()){
                event->ignore();
                return;
            }
            startPoint = endPoint = currentPoint;
            roiRect = getSelectedArea();
            drawCursor();
            emit signalUpdateRoi(roiRect);
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void MyImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!IsNullImage()){
        int x = (int)event->scenePos().x();
        int y = (int)event->scenePos().y();
        QPoint currentPoint = QPoint(x, y);
        // send pixelinfo
        if (currentImage.valid(x, y)){
            QRgb rgb = currentImage.pixel(x, y);
            emit signalPixelInfo(QString::asprintf("X:%4d Y:%4d [R:%3d G:%3d B:%3d]", x, y, qRed(rgb), qGreen(rgb), qBlue(rgb)));
        }

        if (event->buttons() == Qt::LeftButton){
            if (x < 0 || x > currentImage.width() || y < 0 || y > currentImage.height()){
                event->ignore();
                return;
            }
            endPoint = currentPoint;
            if (startPoint.x() > x || startPoint.y() > y)
                endPoint = startPoint;

            roiRect = getSelectedArea();
            drawCursor();
            emit signalUpdateRoi(roiRect);
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}
void MyImageScene::drawCursor()
{
    if (IsNullImage())
        return;

    // remove item except image
    foreach (QGraphicsItem *item, items()){
        if (item->type() != 7)
            removeItem(item);
    }

    if (roiType != SELTYPE_FIX && (startPoint.x() < 0 || startPoint.y() < 0))
        return;

    QPen pen(cursorColor);
    pen.setCosmetic(true);

    int l = 1600 / zoomPercent;
    if (l < 1)
        l = 1;
    if (l > 30)
        l = 30;

    if (roiType == SELTYPE_RECT || roiType == SELTYPE_FIX){
        addRect(roiRect, pen);
    }
    else if (roiType == SELTYPE_HORZ){
        double lineCenter = endPoint.y() + 0.5;
        addLine(0, lineCenter, currentImage.width() - 1, lineCenter, pen);
        if (startPoint != endPoint){
            addLine(startPoint.x() - l, lineCenter - l, startPoint.x(), lineCenter, pen);
            addLine(startPoint.x() - l, lineCenter + l, startPoint.x(), lineCenter, pen);
            addLine(startPoint.x() - l, lineCenter - l, startPoint.x() - l, lineCenter + l, pen);
            addLine(endPoint.x() + l, lineCenter - l, endPoint.x(), lineCenter, pen);
            addLine(endPoint.x() + l, lineCenter + l, endPoint.x(), lineCenter, pen);
            addLine(endPoint.x() + l, lineCenter - l, endPoint.x() + l, lineCenter + l, pen);
        }
    }
    else if (roiType == SELTYPE_VERT){
        double lineCenter = endPoint.x() + 0.5;
        addLine(lineCenter, 0 , lineCenter, currentImage.height() - 1, pen);
        if (startPoint != endPoint){
            addLine(lineCenter - l, startPoint.y() - l , lineCenter, startPoint.y(), pen);
            addLine(lineCenter + l, startPoint.y() - l , lineCenter, startPoint.y(), pen);
            addLine(lineCenter - l, startPoint.y() - l , lineCenter + l, startPoint.y() - l, pen);
            addLine(lineCenter - l, endPoint.y() + l , lineCenter, endPoint.y(), pen);
            addLine(lineCenter + l, endPoint.y() + l , lineCenter, endPoint.y(), pen);
            addLine(lineCenter - l, endPoint.y() + l , lineCenter + l, endPoint.y() + l , pen);
        }
    }
    update();
}

void MyImageScene::allocBuffer()
{
    for (int color = 0; color < NUM_COLOR; ++color){
        pHistBuff[color] = new int [HIST_NUM];
        vGraphY.clear();
    }
    vGraphX.clear();
}

void MyImageScene::freeBuffer()
{
    for (int color = 0; color < NUM_COLOR; ++color){
        if (pHistBuff[color] != NULL){
            delete[] pHistBuff[color];
            pHistBuff[color] = NULL;
        }
    }
}
QRect MyImageScene::getSelectedArea()
{  
    QRect rect;

    switch (roiType) {
    case SELTYPE_RECT:
        if (startPoint.x() < 0 || startPoint.y() < 0)
            return QRect(0, 0, 0, 0);

        rect.setLeft(startPoint.x());
        rect.setTop(startPoint.y());
        rect.setRight(endPoint.x());
        rect.setBottom(endPoint.y());
        break;

    case SELTYPE_FIX:
        // to make sure
        if (fixedArea.left() < 0)       fixedArea.setLeft(0);
        if (fixedArea.top() < 0)        fixedArea.setTop(0);
        if (fixedArea.right() < 0)      fixedArea.setRight(0);
        if (fixedArea.bottom() < 0)     fixedArea.setBottom(0);
        if (!IsNullImage()){
        if (fixedArea.width() + fixedArea.left() > currentImage.width())
            fixedArea.setWidth(currentImage.width() - fixedArea.left());
        if (fixedArea.height() + fixedArea.top() > currentImage.height())
            fixedArea.setHeight(currentImage.height() - fixedArea.top());
        }
        rect.setRect(fixedArea.left(), fixedArea.top(), fixedArea.width(), fixedArea.height());
        break;

    case SELTYPE_HORZ:
        if (startPoint.x() < 0 || startPoint.y() < 0)
            return QRect(0, 0, 0, 0);

        if (startPoint == endPoint){
            rect.setLeft(0);
            rect.setWidth(currentImage.width());
        }
        else {
            rect.setLeft(startPoint.x());
            rect.setWidth(endPoint.x() - startPoint.x());
        }
        rect.setTop(endPoint.y());
        rect.setHeight(1);
        break;

    case SELTYPE_VERT:
        if (startPoint.x() < 0 || startPoint.y() < 0)
            return QRect(0, 0, 0, 0);

        if (startPoint == endPoint){
            rect.setTop(0);
            rect.setHeight(currentImage.height());
        }
        else {
            rect.setTop(startPoint.y());
            rect.setHeight(endPoint.y() - startPoint.y());
        }
        rect.setLeft(endPoint.x());
        rect.setWidth(1);
        break;

    case SELTYPE_FULL:
        rect.setLeft(0);
        rect.setTop(0);
        rect.setWidth(currentImage.width());
        rect.setHeight(currentImage.height());
        break;

    default:
        break;
    }

    return rect;
}


