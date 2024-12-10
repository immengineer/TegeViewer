#include "graphdialog.h"
#include "ui_graphdialog.h"
#include "mainwindow.h"

GraphDialog::GraphDialog(QWidget *parent, int type)
    : QDialog(parent)
    , ui(new Ui::GraphDialog)
    , isShown(false)
    , isDrawingCursor(false)
    , selectionType(SELTYPE_NULL)
    , startPoint(-1, -1)
    , endPoint(-1, -1)
    , autoX(true)
    , autoY(true)
{
    ui->setupUi(this);
    graphType = type;
    selectionType = SELTYPE_NULL;
    Myparent = parentWidget();
    initUI();
    createActions();
}
GraphDialog::~GraphDialog()
{
    delete ui;
}
bool GraphDialog::IsHorizontal()
{
    return ui->radioH->isChecked();
}
bool GraphDialog::IsVertical()
{
    return ui->radioV->isChecked();
}
bool GraphDialog::IsAverageGraph()
{
    return ui->checkBoxAve->isChecked();
}
void GraphDialog::updateRoiArea(QRect rect)
{
    roiRect = rect;

    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);
    roiType = pMain->GetScene()->roiType;

    if (graphType == GRAPHTYPE_HISTO)
        setUpHistoGraph();
    else if (graphType == GRAPHTYPE_LEVEL)
        setUpLevelGraph();

    update();
}

void GraphDialog::changeRoiType(int type)
{
    if (roiType == SELTYPE_HORZ || roiType == SELTYPE_VERT) {
        if (roiType == SELTYPE_HORZ)
            ui->radioH->setChecked(true);
        else
            ui->radioV->setChecked(true);

        directionChanged(true);     // true : dummy
    }

}
void GraphDialog::initUI()
{
    ui->checkBoxR->setChecked(true);
    ui->checkBoxG->setChecked(true);
    ui->checkBoxB->setChecked(true);

    if (graphType == GRAPHTYPE_HISTO){
        ui->labelDirection->hide();
        ui->checkBoxAve->hide();
        ui->radioH->hide();
        ui->radioV->hide();
        ui->labelXAxis->setText("Pixel value");
        showTitle();
        setUpHistoGraph();
    }
    else if (graphType == GRAPHTYPE_LEVEL){
        ui->labelDirection->show();
        ui->checkBoxAve->show();
        ui->radioH->show();
        ui->radioH->setChecked(true);
        ui->labelXAxis->setText("Horizontal pos.");
        ui->radioV->show();       
        showTitle();
        setUpLevelGraph();
    }
}

void GraphDialog::createActions()
{
    connect(ui->checkBoxR, SIGNAL(stateChanged(int)), this, SLOT(update()));
    connect(ui->checkBoxG, SIGNAL(stateChanged(int)), this, SLOT(update()));
    connect(ui->checkBoxB, SIGNAL(stateChanged(int)), this, SLOT(update()));
    connect(ui->pushButtonAuto, SIGNAL(clicked(bool)), this, SLOT(pushButtonAuto_clicked(bool)));
    connect(ui->radioH, SIGNAL(clicked(bool)), this, SLOT(directionChanged(bool)));
    connect(ui->radioV, SIGNAL(clicked(bool)), this, SLOT(directionChanged(bool)));
    connect(ui->checkBoxAve, SIGNAL(clicked(bool)), this, SLOT(averageChanged(bool)));
    connect(ui->pushButtonCopy, SIGNAL(clicked(bool)), this, SLOT(pushButtonCopy_clicked(bool)));
}

void GraphDialog::showTitle()
{
    QString s;
    if (graphType == GRAPHTYPE_HISTO)
        s = "Histogram";
    else if (graphType == GRAPHTYPE_LEVEL) {
        s = "ImageLevel";
    }
    if (selectionType == SELTYPE_HORZ)
        s += "  mouse selection type : Horizontal";
    else if (selectionType == SELTYPE_VERT)
        s += "  mouse selection type : Vertical";
    else if (selectionType == SELTYPE_RECT)
        s += "  mouse selection type : Rectangle";

    setWindowTitle(s);
}

void GraphDialog::reDraw(bool setup)
{
    startPoint.setX(-1);
    startPoint.setY(-1);
    endPoint.setX(-1);
    endPoint.setY(-1);

    if (setup){
        if (graphType == GRAPHTYPE_HISTO)
            setUpHistoGraph();
        else if (graphType == GRAPHTYPE_LEVEL)
            setUpLevelGraph();
    }
    update();
}
void GraphDialog::setUpHistoGraph()
{
    // get pointer to MainWindow
    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);

    if (autoX){
        fXMin = 0;
        fXMax = HIST_NUM - 1;
    }

    bool IsEnableHiso = pMain->GetScene()->CreateHistogramData();
    if (autoY){
        int iVal = IsEnableHiso ? pMain->GetScene()->GetHistogramMaxNum() : 255;
        double fVal;    //dummy
        getScale(0, iVal, &fYMin, &fYMax, &fVal);
    }
}

void GraphDialog::setUpLevelGraph()
{
    // get pointer to MainWindow
    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);

    if (!pMain->GetScene()->IsNullImage() && !pMain->GetScene()->IsInvalidROI())
        pMain->GetScene()->CreateGraphData(IsHorizontal(), IsAverageGraph());

    if (autoX){
        fXMin = IsHorizontal() ? pMain->GetScene()->roiRect.left() : pMain->GetScene()->roiRect.top();
        fXMax = IsHorizontal() ? pMain->GetScene()->roiRect.right() : pMain->GetScene()->roiRect.bottom();
    }
    if (autoY){
        fYMin = 0;
        fYMax = HIST_NUM - 1;
    }
}

void GraphDialog::drawGraphFrame(QPainter *painter)
{
    // Draw Frame
    painter->setBrush(Qt::white);
    painter->drawRect(GRAPH_LEFT, GRAPH_FOOTER, graphWidth, graphHeight);
    painter->setBrush(Qt::transparent);

    if (fXMax == fXMin || fYMax == fYMin)
        return;

    // define variable
    int x, y;
    double scaleMin, scaleMax, reso;
    double index;

    QPen pen;
    pen.setStyle(Qt::DashDotLine);
    painter->setPen(pen);

    // X axis
    y = GRAPH_HEADER + graphHeight + 15;        // for label (reset transform)
    getScale(fXMin, fXMax, &scaleMin, &scaleMax, &reso);
    fXRange = fXMax- fXMin;
    if (reso < 1.0)
        reso = 1.0;

    for (index = scaleMin; index <= scaleMax; index += reso)  {
        if (index < fXMin || index > fXMax)
            continue;

        x = (int)((double)graphWidth * ((index - fXMin) / fXRange)) + GRAPH_LEFT;
        painter->drawLine(x, GRAPH_FOOTER, x, GRAPH_FOOTER + graphHeight);

        QString s = QString::number((int)index);
        int margin = 4 * (s.size() - 1);
        painter->save();
        painter->resetTransform();
        painter->drawText(x - margin, y, s);
        painter->restore();
    }

    // Y axis
    x = GRAPH_LEFT - 10;
    getScale(fYMin, fYMax, &scaleMin, &scaleMax, &reso);
    fYRange = fYMax- fYMin;
    if (reso < 1.0)
        reso = 1.0;

    for (index = scaleMin; index <= scaleMax; index += reso)  {
        if (index < fYMin || index > fYMax)
            continue;

        y = (int)((double)graphHeight * ((index - fYMin) / fYRange)) + GRAPH_FOOTER;
        painter->drawLine(GRAPH_LEFT, y, GRAPH_LEFT + graphWidth, y);

        QString s = QString::number((int)index);
        int margin = 5 * (s.size() - 1);
        painter->save();
        painter->resetTransform();
        painter->drawText(x - margin, graphHeight + GRAPH_HEADER + GRAPH_FOOTER- y + 4, s);
        painter->restore();
    }
}

void GraphDialog::drawHistoGraphLine(QPainter *painter)
{
    // get pointer to MainWindow
    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);

    if (pMain->GetScene()->IsNullImage())
        return;
    if (pMain->GetScene()->IsInvalidROI())
        return;

    int x, color;
    int iVal;

    QPen pen[NUM_COLOR];
    pen[COLOR_INDEX_R].setColor(Qt::red);
    pen[COLOR_INDEX_G].setColor(Qt::green);
    pen[COLOR_INDEX_B].setColor(Qt::blue);

    bool draw[NUM_COLOR];
    draw[COLOR_INDEX_R] = ui->checkBoxR->isChecked();
    draw[COLOR_INDEX_G] = ui->checkBoxG->isChecked();
    draw[COLOR_INDEX_B] = ui->checkBoxB->isChecked();

    int iXMin = (int)fXMin;
    int iXMax = (int)fXMax;
    int iYMin = (int)fYMin;
    int iYMax = (int)fYMax;

    for (color = 0; color < NUM_COLOR; ++color){
        if (draw[color]){
            painter->setPen(pen[color]);
            int count = 0;

            QPainterPath path;
            QPoint pos;
            for (x = iXMin; x <= iXMax; ++x){
                iVal = pMain->GetScene()->GetHistogramData(x, color);
                if (iVal >= iYMin && iVal <= iYMax){
                    pos.setX((int)((double)graphWidth * ((x - iXMin) / fXRange)) + GRAPH_LEFT);
                    pos.setY((int)((double)graphHeight * ((iVal - iYMin) / fYRange)) + GRAPH_FOOTER);
                    if (count == 0)
                        path.moveTo(pos);
                    else
                        path.lineTo(pos);
                    count++;
                }
            }
            if (path.length() > 0)
                painter->drawPath(path);
        }
    }
}

void GraphDialog::drawLevelGraphLine(QPainter *painter)
{
    // get pointer to MainWindow
    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);

    if (pMain->GetScene()->IsNullImage())
        return;
    if (pMain->GetScene()->IsInvalidROI())
        return;

    int data = pMain->GetScene()->numOfGraphData;
    int series = pMain->GetScene()->numOfGraphSeries;

    double xRatio = (double)graphWidth / fXRange;
    double yRatio = (double)graphHeight / fYRange;

    QPen pen[NUM_COLOR];
    pen[COLOR_INDEX_R].setColor(Qt::red);
    pen[COLOR_INDEX_G].setColor(Qt::green);
    pen[COLOR_INDEX_B].setColor(Qt::blue);

    bool draw[NUM_COLOR];
    draw[COLOR_INDEX_R] = ui->checkBoxR->isChecked();
    draw[COLOR_INDEX_G] = ui->checkBoxG->isChecked();
    draw[COLOR_INDEX_B] = ui->checkBoxB->isChecked();

    for (int color = 0; color < NUM_COLOR; ++color){
        if (!draw[color])
            continue;

        painter->setPen(pen[color]);

        for (int j = 0; j < series; ++j){
            QPainterPath path;
            QPoint pos;
            int count = 0;
            for (int i = 0; i < data; ++i) {
                int x = pMain->GetScene()->vGraphX[i];
                int index = (j * data) + i;
                QRgb rgb = pMain->GetScene()->vGraphY[index];
                int y = 0;
                if (color == COLOR_INDEX_R)
                    y = qRed(rgb);
                else if (color == COLOR_INDEX_G)
                    y = qGreen(rgb);
                else if (color == COLOR_INDEX_B)
                    y = qBlue(rgb);


                if (x >= fXMin && x <= fXMax && y >= fYMin && y <= fYMax) {
                    pos.setX((int)(xRatio * ((double)x - fXMin)) + GRAPH_LEFT);
                    pos.setY((int)(yRatio * ((double)y - fYMin)) + GRAPH_FOOTER);

                    if (count == 0)
                        path.moveTo(pos);
                    else
                        path.lineTo(pos);

                    count++;
                }
            }
            if (path.length() > 0)
                painter->drawPath(path);
        }
    }
}
void GraphDialog::getScale(double min, double max, double *scaleMin, double *scaleMax, double *reso)
{
    if (min == max){
        if (max == 0){
            max = 1;
        }
        else if (max > 0)
            min = 0;
        else
            max = 0;
    }
    double range = max - min;
    double ext = log10(range);

    double man;
    if (ext < 0)
        man = pow(10.0, fmod(ext, 1.0) + 1.0);
    else
        man = pow(10.0, fmod(ext, 1.0));

    double org;
    if (man < 1.1)
        org = 0.1;
    else if (man < 2.2)
        org = 0.2;
    else if (man < 5.5)
        org = 0.5;
    else
        org = 1;

    if (ext < 0)
        *reso = org * pow((double)10, ceil(ext-1.0));
    else
        *reso = org * pow((double)10, floor(ext));
    *scaleMin = *reso == 0 ? 0 : floor(min / *reso) * *reso;

    *scaleMax = (fmod(max / *reso, 1) == 0) ? max : (floor(max / *reso) + 1.0) * *reso;
}

void GraphDialog::inputScale()
{
    minmaxDialog = new MinMaxDialog(this);
    minmaxDialog->XMin = (int)fXMin;
    minmaxDialog->XMax = (int)fXMax;
    minmaxDialog->YMin = (int)fYMin;
    minmaxDialog->YMax = (int)fYMax;
    minmaxDialog->setModal(true);
    minmaxDialog->setFixedSize(170, 150);

    if (minmaxDialog->exec() == QDialog::Accepted){
        fXMin = minmaxDialog->XMin;
        fXMax = minmaxDialog->XMax;
        fYMin = minmaxDialog->YMin;
        fYMax = minmaxDialog->YMax;

        if (graphType == GRAPHTYPE_HISTO){
            if (fXMin < 0)
                fXMin = 0;
            if (fXMax > HIST_NUM - 1)
                fXMin = HIST_NUM - 1;
        }
        else if (graphType == GRAPHTYPE_LEVEL) {
            if (IsHorizontal()) {
                if (fXMin < roiRect.left())
                    fXMin = roiRect.left();
                if (fXMax > roiRect.right())
                    fXMax = roiRect.right();
            }
            else {
                if (fXMin < roiRect.top())
                    fXMin = roiRect.top();
                if (fXMax > roiRect.bottom())
                    fXMax = roiRect.bottom();
            }
            if (fYMin < 0)
                fYMin = 0;
            if (fYMax > HIST_NUM - 1)
                fYMin = HIST_NUM - 1;
        }
    }
    delete minmaxDialog;
}

bool GraphDialog::getGraphPos(QPointF mousepos, QPointF *pos)
{
    pos->setX(mousepos.x() - GRAPH_LEFT);
    pos->setY(this->height() - GRAPH_FOOTER - mousepos.y());

    return (pos->x() > 0 && pos->x() < graphWidth && pos->y() > 0 && pos->y() < graphHeight);
}

void GraphDialog::drawCursor(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::black);
    painter->setPen(pen);
    int l = 10;         // arrow size
    QPoint drawStart, drawEnd;
    drawStart.setX(startPoint.x() + GRAPH_LEFT);
    drawStart.setY(startPoint.y() + GRAPH_FOOTER);
    drawEnd.setX(endPoint.x() + GRAPH_LEFT);
    drawEnd.setY(endPoint.y() + GRAPH_FOOTER);
    QList<QLine> drawLines;

    if (selectionType == SELTYPE_HORZ) {
        int yMinus = drawEnd.y() - l;
        int yPlus = drawEnd.y() + l;
        drawLines.append(QLine(GRAPH_LEFT, drawEnd.y(), GRAPH_LEFT + graphWidth, drawEnd.y()));
        if (drawStart.x() != drawEnd.x()){
            drawLines.append(QLine(drawStart.x() - l, yMinus, drawStart.x(), drawEnd.y()));
            drawLines.append(QLine(drawStart.x() - l, yPlus, drawStart.x(), drawEnd.y()));
            drawLines.append(QLine(drawStart.x() - l, yMinus, drawStart.x() - l, yPlus));
            drawLines.append(QLine(drawStart.x(), yMinus, drawStart.x(), yPlus));
            drawLines.append(QLine(drawEnd.x() + l, yMinus, drawEnd.x(), drawEnd.y()));
            drawLines.append(QLine(drawEnd.x() + l, yPlus, drawEnd.x(), drawEnd.y()));
            drawLines.append(QLine(drawEnd.x() + l, yMinus, drawEnd.x() + l, yPlus));
            drawLines.append(QLine(drawEnd.x(), yMinus, drawEnd.x(), yPlus));
        }
    }
    else if (selectionType == SELTYPE_VERT) {
        int xMinus = drawEnd.x() - l;
        int xPlus = drawEnd.x() + l;
        drawLines.append(QLine(drawEnd.x(), GRAPH_FOOTER, drawEnd.x(), GRAPH_FOOTER + graphHeight));
        if (drawStart.y() != drawEnd.y()){
            drawLines.append(QLine(xMinus, drawStart.y() + l, drawEnd.x(), drawStart.y()));
            drawLines.append(QLine(xPlus, drawStart.y() + l, drawEnd.x(), drawStart.y()));
            drawLines.append(QLine(xMinus, drawStart.y() + l, xPlus, drawStart.y() + l));
            drawLines.append(QLine(xMinus, drawStart.y(), xPlus, drawStart.y()));

            drawLines.append(QLine(xMinus, drawEnd.y() - l, drawEnd.x(), drawEnd.y()));
            drawLines.append(QLine(xPlus, drawEnd.y() - l, drawEnd.x(), drawEnd.y()));
            drawLines.append(QLine(xMinus, drawEnd.y() - l, xPlus, drawEnd.y() - l));
            drawLines.append(QLine(xMinus, drawEnd.y(), xPlus, drawEnd.y()));
        }
    }
    else if (selectionType == SELTYPE_RECT) {
        if (drawStart != drawEnd){
            drawLines.append(QLine(drawStart.x(), drawStart.y(), drawEnd.x(), drawStart.y()));
            drawLines.append(QLine(drawEnd.x(), drawStart.y(), drawEnd.x(), drawEnd.y()));
            drawLines.append(QLine(drawEnd.x(), drawEnd.y(), drawStart.x(), drawEnd.y()));
            drawLines.append(QLine(drawStart.x(), drawEnd.y(), drawStart.x(), drawStart.y()));
        }
    }
    painter->drawLines(drawLines);
}
void GraphDialog::pushButtonAuto_clicked(bool bVal)
{
    Q_UNUSED(bVal);
    autoX = true;
    autoY = true;
    reDraw();
}

void GraphDialog::pushButtonCopy_clicked(bool bVal)
{
    Q_UNUSED(bVal);

    // get pointer to MainWindow
    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);

    if (pMain->GetScene()->IsNullImage())
        return;
    if (pMain->GetScene()->IsInvalidROI())
        return;


    QClipboard *clipboard = QApplication::clipboard();
    QString s = "";
    if (graphType == GRAPHTYPE_HISTO){
        for (int i = 0; i < HIST_NUM; ++i){
            s += QString::number(i) + TAB;
            s += QString::number(pMain->GetScene()->GetHistogramData(i, COLOR_INDEX_R)) + TAB;
            s += QString::number(pMain->GetScene()->GetHistogramData(i, COLOR_INDEX_G)) + TAB;
            s += QString::number(pMain->GetScene()->GetHistogramData(i, COLOR_INDEX_B)) + LF;
        }
        clipboard->setText(s);
    }
    else if (graphType == GRAPHTYPE_LEVEL){
        int data = pMain->GetScene()->numOfGraphData;
        int series = pMain->GetScene()->numOfGraphSeries;

        if (series > GRAPH_SERIES_MAX)
            series = GRAPH_SERIES_MAX;

        for (int i = 0; i < data; ++i) {
            s += QString::number(pMain->GetScene()->vGraphX[i]) + TAB;
            for (int j = 0; j < series; ++j) {
                int index = (j * data) + i;
                QRgb rgb = pMain->GetScene()->vGraphY[index];
                s += QString::number(qRed(rgb)) + TAB;
                s += QString::number(qGreen(rgb)) + TAB;
                s += QString::number(qBlue(rgb));

                if (j == series - 1)
                    s += LF;
                else
                    s += TAB;
            }
        }
        clipboard->setText(s);
    }
}
void GraphDialog::directionChanged(bool bVal)
{
    Q_UNUSED(bVal);
    if (IsHorizontal())
        ui->labelXAxis->setText("Horizontal pos.");
    else
        ui->labelXAxis->setText("Vertical pos.");

    reDraw();
}

void GraphDialog::averageChanged(bool bVal)
{
    Q_UNUSED(bVal);
    reDraw();
}
void GraphDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.translate(0, this->height());
    painter.scale(1, -1);
    drawGraphFrame(&painter);
    if (graphType == GRAPHTYPE_HISTO)
        drawHistoGraphLine(&painter);
    else if (graphType == GRAPHTYPE_LEVEL)
        drawLevelGraphLine(&painter);

    if (startPoint != endPoint){       
        drawCursor(&painter);
    }
}

void GraphDialog::closeEvent(QCloseEvent *event)
{
    isShown = false;
}

void GraphDialog::showEvent(QShowEvent *event)
{
    isShown = true;
}

void GraphDialog::hideEvent(QHideEvent *event)
{
    isShown = false;
}

void GraphDialog::resizeEvent(QResizeEvent *event)
{
    QSize size(this->width() - (GRAPH_LEFT + GRAPH_RIGHT), this->height() - (GRAPH_HEADER + GRAPH_FOOTER));
    graphWidth = size.width();
    graphHeight = size.height();
    ui->labelXAxis->move(QPoint(this->width() / 2, this->height() - 20));
    ui->pushButtonAuto->move(QPoint(this->width() - 80, 3));
    ui->pushButtonCopy->move(QPoint(this->width() - 150, 3));

    startPoint.setX(-1);
    startPoint.setY(-1);
    endPoint.setX(-1);
    endPoint.setY(-1);
}

void GraphDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton && startPoint != endPoint){
        if (selectionType == SELTYPE_HORZ && startPoint.x() > 0){
            double ratio = fXRange /(double)graphWidth;
            int min = (int)(fXMin + (startPoint.x() * ratio));
            int max = (int)(fXMin + (endPoint.x() * ratio));
            fXMin = min;
            fXMax = max;
            autoX = false;
            reDraw();
        }
        else if (selectionType == SELTYPE_VERT && startPoint.x() > 0){
            double ratio = fYRange /(double)graphHeight;
            int min = (int)(fYMin + (endPoint.y() * ratio));
            int max = (int)(fYMin + (startPoint.y() * ratio));
            fYMin = min;
            fYMax = max;
            autoY = false;
            reDraw();
        }
        else if (selectionType == SELTYPE_RECT && startPoint.x() > 0){
            double xratio = fXRange /(double)graphWidth;
            int xmin = (int)(fXMin + (startPoint.x() * xratio));
            int xmax = (int)(fXMin + (endPoint.x() * xratio));
            fXMin = xmin;
            fXMax = xmax;
            autoX = false;

            double yratio = fYRange /(double)graphHeight;
            int ymin = (int)(fYMin + (endPoint.y() * yratio));
            int ymax = (int)(fYMin + (startPoint.y() * yratio));
            fYMin = ymin;
            fYMax = ymax;
            autoY = false;
            reDraw();
        }
    }
    else if (event->button() == Qt::LeftButton){
        QPointF currentPoint;
        if (getGraphPos(event->scenePosition(), &currentPoint)){
            startPoint = currentPoint;
            endPoint = currentPoint;
            isDrawingCursor = true;
            update();
        }
    }
    QDialog::mousePressEvent(event);
}

void GraphDialog::mouseMoveEvent(QMouseEvent *event)
{
    QPointF currentPoint;
    if (isDrawingCursor && getGraphPos(event->scenePosition(), &currentPoint)){
        endPoint = currentPoint;
        if (selectionType == SELTYPE_HORZ || selectionType == SELTYPE_RECT)
        {
            if (startPoint.x() > currentPoint.x())
                endPoint = startPoint;
        }

        if (selectionType == SELTYPE_VERT || selectionType == SELTYPE_RECT)
        {
            if (startPoint.y() < currentPoint.y())
                endPoint = startPoint;
        }

        update();
    }
    QDialog::mouseMoveEvent(event);
}
void GraphDialog::mouseReleaseEvent(QMouseEvent *event)
{
    isDrawingCursor = false;
    QDialog::mouseReleaseEvent(event);
}
void GraphDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
    case Qt::Key_H:
        selectionType = SELTYPE_HORZ;
        showTitle();
        reDraw(false);
        break;

    case Qt::Key_V:
        selectionType = SELTYPE_VERT;
        showTitle();
        reDraw(false);
        break;

    case Qt::Key_R:
        selectionType = SELTYPE_RECT;
        showTitle();
        reDraw(false);
        break;

    case Qt::Key_A:
        autoX = true;
        autoY = true;
        reDraw();
        break;

    case Qt::Key_I:
        inputScale();
        autoX = false;
        autoY = false;
        reDraw();
        break;

    default:
        QDialog::keyPressEvent(event);
        break;
    }
}

void GraphDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        autoX = true;
        autoY = true;
        reDraw();
    }
}
