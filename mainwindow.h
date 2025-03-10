#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QLabel>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QApplication>
#include <QStyle>
#include <QMouseEvent>
#include <QtWidgets>
#include <QtCore>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

#include <opencv2/opencv.hpp>

#include "myimageview.h"
#include "myimagescene.h"
#include "define.h"
#include "graphdialog.h"
#include "infodock.h"
#include "utility.h"
#include "cameracontroldialog.h"
#include "cvvideocapture.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MyImageScene*   GetScene() {return imageScene;  }
    MyImageView*    GetView()  {return imageView;   }

    inline bool IsHorizontal() { return dialogGraph->IsHorizontal();}
    inline bool IsVertical() { return dialogGraph->IsVertical();}
    inline bool IsAverageGraph() { return dialogGraph->IsAverageGraph();}

signals:
    void signalChangeRoiType(int type);

private:
    void initUI();
    void createDock();
    void createActions();
    void showImage(QString path);
    void showTitle();
    void enabler(bool enable);

    QRect getSelectedArea();

private slots:
    void openImage();
    void saveAs();
    void zoomIn();
    void zoomOut();
    void fitWindow();
    void zoomOriginal();
    void showPixelInfo(QString sInfo);
    void showDroppedImage(QList<QUrl> urllist);
    void showZoomRatio(qreal ratio);
    void showPreviousImage();
    void showNextImage();
    void showHistogramDialog();
    void showGraphDialog();
    void showCameraControlDialog();
    void selRectangle();
    void selFixed();
    void selHorizontal();
    void selVertical();
    void selFull();
    void changeCursorColor();
    void about();
    void showCameraImage(QVideoFrame* frame);
    void detectCamera(bool IsReady);
    void startCamera();
    void stopCamera();
    void dispFps();
    void showCvCaptureImage();

private:
    // Menu, ToolBar, StatusBar
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *analysisMenu;
    QMenu *roiTypeMenu;
    QMenu *cameraMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *analysisToolBar;
    QToolBar *cameraToolBar;
    QStatusBar *mainStatusBar;
    QLabel *imageInfoLabel;
    QLabel *zoomInfoLabel;
    QLabel *camInfoLabel;

    // Customize QGraphicsView QGraphicsScene
    MyImageScene *imageScene;
    MyImageView *imageView;

    // Dialog
    GraphDialog *dialogHistoGram;
    GraphDialog *dialogGraph;

    InfoDock *infoDock;

    QAction *openAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *fitWindowAction;
    QAction *zoomOriginalAction;
    QAction *nextImageAction;
    QAction *previousImageAction;
    QAction *infoDialogAction;
    QAction *histDialogAction;
    QAction *graphDialogAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *aboutQCameraControlAction;

    QAction *selRectangleAction;
    QAction *selFixedAction;
    QAction *selHorizontalAction;
    QAction *selVerticalAction;
    QAction *selFullAction;
    QAction *changeCursorColorAction;

    QAction *detectCameraAction;
    QAction *startCameraAction;
    QAction *stopCameraAction;

    // Open file
    QString currentImagePath;

    // Drag and Drop files
    QList<QString> imageFilePathList;
    int32_t imageIndex;

    // UVC Camera
    CameraControlDialog *camControl;

    QString cameraDescrption;
    QString pixelFormat;
    // volatile bool IsCameraStarted;
    QTimer *fpsTimer;

    // OpenCV VideoCapture
    CvVideoCapture *videoCapture;
};
#endif // MAINWINDOW_H
