#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QRect>
#include <QLabel>
#include <QTransform>
#include <QString>
#include <QAbstractButton>
#include "define.h"
#include "minmaxdialog.h"

namespace Ui {
class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphDialog(QWidget *parent, int type);
    ~GraphDialog();

    inline bool IsShown(){return isShown;}
    bool IsHorizontal();
    bool IsVertical();
    bool IsAverageGraph();

public slots:
    void updateRoiArea(QRect rect);
    void changeRoiType(int type);

private:
    Ui::GraphDialog *ui;
    QWidget* Myparent;
    MinMaxDialog* minmaxDialog;

    bool isShown;
    bool isDrawingCursor;
    int graphWidth;
    int graphHeight;
    int graphType;
    int selectionType;
    QRect roiRect;
    int roiType;

    double fXMin, fXMax, fYMin, fYMax;
    double fXRange, fYRange;
    QPointF startPoint, endPoint;
    bool autoX, autoY;

    void initUI();
    void createActions();
    void showTitle();

    void reDraw(bool setup = true);
    void setUpHistoGraph();
    void setUpLevelGraph();
    void drawGraphFrame(QPainter *painter);
    void drawHistoGraphLine(QPainter *painter);
    void drawLevelGraphLine(QPainter *painter);
    void getScale(double min, double max, double *scaleMin, double *scaleMax, double *reso);
    void inputScale();
    bool getGraphPos(QPointF mousepos, QPointF *pos);
    void drawCursor(QPainter *painter);

private slots:
    void pushButtonAuto_clicked(bool bVal);
    void pushButtonCopy_clicked(bool bVal);
    void averageChanged(bool bVal);
    void directionChanged(bool bVal);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // GRAPHDIALOG_H
