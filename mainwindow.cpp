#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , fileMenu(nullptr)
    , viewMenu(nullptr)
    , analysisMenu(nullptr)
    , cameraMenu(nullptr)
    , helpMenu(nullptr)
{
    initUI();
    createDock();
    createActions();
    imageFilePathList.clear();
    imageIndex = -1;
    cameraDescrption = "";
    IsCameraStarted = false;
    showTitle();
}
MainWindow::~MainWindow() {}

void MainWindow::initUI()
{
    this->resize(STARTUP_WIDTH, STARTUP_HEIGHT);

    // setup menubar
    fileMenu = menuBar()->addMenu(tr("&File"));
    viewMenu = menuBar()->addMenu(tr("&View"));
    analysisMenu = menuBar()->addMenu(tr("&Analysis"));
    cameraMenu = menuBar()->addMenu(tr("&Camera"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    // setup toolbar
    fileToolBar = addToolBar("File");
    viewToolBar = addToolBar("View");
    analysisToolBar = addToolBar("Analysis");

    // main area for image display
    imageScene = new MyImageScene(this);
    imageView = new MyImageView(imageScene);
    imageView->setBackgroundBrush(QBrush(Qt::gray));

    // set Widget
    setCentralWidget(imageView);

    // setup View
    imageView->installEventFilter(this);
    imageView->setMouseTracking(true);          // Viewのコンストラクタ内に記述すると効かない？ようなので、ここで指定する
    imageView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    imageView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    imageView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    imageView->zoomLimitMin = ZOOMLIMIT_MIN;
    imageView->zoomLimitMax = ZOOMLIMIT_MAX;

    // setup Scene
    imageScene->roiType = SELTYPE_RECT;

    // dialog
    dialogHistoGram = new GraphDialog(this, GRAPHTYPE_HISTO);
    dialogHistoGram->setMinimumSize(150, 150);
    dialogGraph = new GraphDialog(this, GRAPHTYPE_LEVEL);
    dialogGraph->setMinimumSize(150, 150);

    // setup status bar
    mainStatusBar = statusBar();
    imageInfoLabel = new QLabel(mainStatusBar);
    mainStatusBar->insertPermanentWidget(0,imageInfoLabel);
    zoomInfoLabel = new QLabel(mainStatusBar);
    mainStatusBar->insertPermanentWidget(1, zoomInfoLabel);

    // icon
    QIcon Icon = style()->standardIcon(QStyle::SP_TitleBarMenuButton);
    QApplication::setWindowIcon(Icon);
}

void MainWindow::createDock()
{
    infoDock = new InfoDock(this);
    infoDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, infoDock);
    infoDock->setFeatures(infoDock->features() & ~QDockWidget::DockWidgetClosable);
    infoDock->setMinimumSize(270, 500);
}
void MainWindow::createActions()
{
    // create actions
    openAction = new QAction("&Open", this);
    openAction->setIcon(QIcon::fromTheme("document-open"));
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));

    saveAsAction = new QAction("&Save as", this);
    saveAsAction->setIcon(QIcon::fromTheme("document-save-as"));
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save as file"));

    exitAction = new QAction("&Exit", this);
    exitAction->setIcon(QIcon::fromTheme("application-exit"));
    exitAction->setStatusTip(tr("Exit Application"));

    zoomInAction = new QAction("Zoom in", this);
    zoomInAction->setIcon(QIcon::fromTheme(("zoom-in")));
    zoomInAction->setShortcuts(QKeySequence::ZoomIn);
    zoomInAction->setStatusTip(tr("Zoom in display image"));

    zoomOutAction = new QAction("Zoom Out", this);
    zoomOutAction->setIcon(QIcon::fromTheme(("zoom-out")));
    zoomOutAction->setShortcuts(QKeySequence::ZoomOut);
    zoomOutAction->setStatusTip(tr("Zoom out display image"));

    fitWindowAction = new QAction("Fit to window", this);
    fitWindowAction->setIcon(QIcon::fromTheme(("zoom-fit-best")));
    fitWindowAction->setShortcut('F');
    fitWindowAction->setStatusTip(tr("Fit to the window display image"));

    zoomOriginalAction = new QAction("Org.", this);
    // zoomOriginalAction->setIcon(QIcon::fromTheme("zoom-original"));  not available zoom-original icon
    zoomOriginalAction->setShortcut('O');
    zoomOriginalAction->setStatusTip(tr("Display image original size"));

    previousImageAction = new QAction("Previous image", this);
    previousImageAction->setIcon(QIcon::fromTheme(("go-previous")));
    previousImageAction->setShortcut(Qt::Key_Left);
    previousImageAction->setStatusTip(tr("Open previous image file"));

    nextImageAction = new QAction("Next image", this);
    nextImageAction->setIcon(QIcon::fromTheme(("go-next")));
    nextImageAction->setShortcut(Qt::Key_Right);
    nextImageAction->setStatusTip(tr("Open next image file"));

    graphDialogAction = new QAction("Level.", this);
    graphDialogAction->setStatusTip(tr("Show / Hide Graph Dialog"));

    histDialogAction = new QAction("Hist.", this);
    histDialogAction->setStatusTip(tr("Show / Hide Histogram Dialog"));

    aboutAction = new QAction("About TegeViewer", this);
    aboutAction->setStatusTip(tr("About TegeViewer"));
    aboutQtAction = new QAction("AboutQt", this);
    aboutQtAction->setStatusTip(tr("About Qt"));

    selRectangleAction = new QAction("Rectangle", this);    selRectangleAction->setShortcut('R');
    selFixedAction = new QAction("FixedSize", this);
    selHorizontalAction = new QAction("Horizontal", this);  selHorizontalAction->setShortcut('H');
    selVerticalAction = new QAction("Vertical", this);      selVerticalAction->setShortcut('V');
    selFullAction = new QAction("Full", this);

    changeCursorColorAction = new QAction("Cursor color", this);
    changeCursorColorAction->setStatusTip(tr("Change Cursor color (default:gray)"));
    changeCursorColorAction->setShortcut('C');

    detectCameraAction = new QAction("Detect Camera", this);
    detectCameraAction->setStatusTip(tr("Detect UVC Camera"));
    detectCameraAction->setShortcut('D');
    startCameraAction = new QAction("Start Camera", this);
    startCameraAction->setStatusTip(tr("Start Camera"));
    stopCameraAction = new QAction("Stop Camera", this);
    stopCameraAction->setStatusTip(tr("Stop Camera"));

    // add actions to menu, toolbar
    fileMenu->addAction(openAction);    fileToolBar->addAction(openAction);
    fileMenu->addAction(saveAsAction);  fileToolBar->addAction(saveAsAction);
    fileMenu->addAction(exitAction);

    viewMenu->addAction(zoomInAction);          viewToolBar->addAction(zoomInAction);
    viewMenu->addAction(zoomOutAction);         viewToolBar->addAction(zoomOutAction);
    viewMenu->addAction(fitWindowAction);       viewToolBar->addAction(fitWindowAction);
    viewMenu->addAction(zoomOriginalAction);    viewToolBar->addAction(zoomOriginalAction);
    viewMenu->addAction(previousImageAction);   viewToolBar->addAction(previousImageAction);
    viewMenu->addAction(nextImageAction);       viewToolBar->addAction(nextImageAction);

    analysisMenu->addAction(graphDialogAction);  analysisToolBar->addAction(graphDialogAction);
    analysisMenu->addAction(histDialogAction);  analysisToolBar->addAction(histDialogAction);

    QMenu *typeMenu = analysisMenu->addMenu("ROI SelectionType");
    typeMenu->addAction(selRectangleAction);
    typeMenu->addAction(selFixedAction);
    typeMenu->addAction(selHorizontalAction);
    typeMenu->addAction(selVerticalAction);
    typeMenu->addAction(selFullAction);

    analysisMenu->addAction(changeCursorColorAction);

    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    cameraMenu->addAction(detectCameraAction);
    cameraMenu->addAction(startCameraAction);
    cameraMenu->addAction(stopCameraAction);

    // connect the signals and slots
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openImage()));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
    connect(fitWindowAction, SIGNAL(triggered(bool)), this, SLOT(fitWindow()));
    connect(zoomOriginalAction, SIGNAL(triggered(bool)), this, SLOT(zoomOriginal()));
    connect(previousImageAction, SIGNAL(triggered(bool)), this, SLOT(showPreviousImage()));
    connect(nextImageAction, SIGNAL(triggered(bool)), this, SLOT(showNextImage()));

    connect(graphDialogAction, SIGNAL(triggered(bool)), this, SLOT(showGraphDialog()));
    connect(histDialogAction, SIGNAL(triggered(bool)), this, SLOT(showHistogramDialog()));
    connect(selRectangleAction, SIGNAL(triggered(bool)), this, SLOT(selRectangle()));
    connect(selFixedAction, SIGNAL(triggered(bool)), this, SLOT(selFixed()));
    connect(selHorizontalAction, SIGNAL(triggered(bool)), this, SLOT(selHorizontal()));
    connect(selVerticalAction, SIGNAL(triggered(bool)), this, SLOT(selVertical()));
    connect(selFullAction, SIGNAL(triggered(bool)), this, SLOT(selFull()));
    connect(changeCursorColorAction, SIGNAL(triggered(bool)), this, SLOT(changeCursorColor()));

    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
    connect(aboutQtAction, &QAction::triggered, QApplication::aboutQt);

    connect(detectCameraAction, SIGNAL(triggered(bool)), this, SLOT(detectCamera()));
    connect(startCameraAction, SIGNAL(triggered(bool)), this, SLOT(startCamera()));
    connect(stopCameraAction, SIGNAL(triggered(bool)), this, SLOT(stopCamera()));

    // from Scene / View
    connect(imageScene, SIGNAL(signalPixelInfo(QString)), this, SLOT(showPixelInfo(QString)));
    connect(imageView, SIGNAL(signalDroppedURLs(QList<QUrl>)), this, SLOT(showDroppedImage(QList<QUrl>)));
    connect(imageView, SIGNAL(signalZoomRatio(qreal)), this, SLOT(showZoomRatio(qreal)));  
    connect(imageScene, SIGNAL(signalUpdateRoi(QRect)), infoDock, SLOT(updateRoiArea(QRect)));
    connect(imageScene, SIGNAL(signalUpdateRoi(QRect)), dialogHistoGram, SLOT(updateRoiArea(QRect)));
    connect(imageScene, SIGNAL(signalUpdateRoi(QRect)), dialogGraph, SLOT(updateRoiArea(QRect)));

    // from infoDock
    connect(infoDock, SIGNAL(signalChangeRoiType(int)), imageScene, SLOT(changeRoiType(int)));
    connect(infoDock, SIGNAL(signalChangeRoiType(int)), dialogGraph, SLOT(changeRoiType(int)));

    // disable action (no image at this moment)
    enabler(false);
    startCameraAction->setEnabled(false);
    stopCameraAction->setEnabled(false);
}

void MainWindow::detectCamera()
{
    camera = nullptr;
    sink = nullptr;
    cameraDescrption = "";
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    if (availableCameras.size() ==  0){
        QMessageBox::warning(this, "Warning", "No Camera found.");
        return;
    }
    // Set first camera
    camera = new QCamera(availableCameras.at(0));
    cameraDescrption = availableCameras.at(0).description();
    // Is Available?
    if (!camera->isAvailable()) {
        QMessageBox::warning(this, "Warning", cameraDescrption + " is not available.");
        return;
    }
    captureSession.setCamera(camera);
    QVideoSink* sink = new QVideoSink;
    captureSession.setVideoSink(sink);

    // camera->start();
    connect(sink, &QVideoSink::videoFrameChanged, this, showCameraImage);
    startCameraAction->setCheckable(true);
    stopCameraAction->setEnabled(false);
    startCameraAction->setEnabled(true);
    showTitle();
}

void MainWindow::startCamera()
{
    openAction->setEnabled(false);
    startCameraAction->setChecked(true);
    startCameraAction->setEnabled(false);
    stopCameraAction->setEnabled(true);
    camera->start();
}

void MainWindow::stopCamera()
{
    openAction->setEnabled(true);
    startCameraAction->setChecked(false);
    startCameraAction->setEnabled(true);
    stopCameraAction->setEnabled(false);
    camera->stop();
    IsCameraStarted = false;
}
void MainWindow::openImage()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle("Open Image");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    QStringList filePaths;
    if (dialog.exec()) {
        filePaths = dialog.selectedFiles();
        showImage(filePaths.at(0));
        imageFilePathList.clear();
        imageIndex = -1;
    }
}
void MainWindow::showImage(QString path)
{
    bool isSameSize;
    // Set pixmap
    QPixmap pixmap(path);
    imageScene->SetPixmap(pixmap, &isSameSize);
    currentImagePath = path;

    // Init View
    imageView->resetTransform();
    imageView->InitZoomLevel();
    imageView->setSceneRect(pixmap.rect());

    fitWindow();
    showTitle();

    if (!imageScene->IsNullImage()){
        enabler(true);
        if (!isSameSize)
            infoDock->ActivateDock(SELTYPE_RECT);
    }
}

void MainWindow::showTitle()
{
    QString sTitle = tr(APP_NAME) + " Ver" + tr(APP_VERSION);

    // Camera available?
    if (cameraDescrption != "")
        sTitle += " : " + cameraDescrption + " ";

    // No image
    if (imageScene->IsNullImage()){
        setWindowTitle(sTitle);
        return;
    }    
    // + image info
    QString sInfo = QString("  %1  %2 x %3   ").arg(currentImagePath).arg(imageScene->GetImageWidth()).arg(imageScene->GetImageHeight());

    QLocale locale;
    sInfo += locale.toString(QFile(currentImagePath).size()) + " Bytes";

    int num = imageFilePathList.size();
    if (num > 0)
        setWindowTitle(sTitle + sInfo + " : " + QString::number(imageIndex + 1) + " / "  + QString::number(num));
    else
        setWindowTitle(sTitle + sInfo);
}

void MainWindow::enabler(bool enable)
{
    saveAsAction->setEnabled(enable);
    zoomInAction->setEnabled(enable);
    zoomOutAction->setEnabled(enable);
    fitWindowAction->setEnabled(enable);
    zoomOriginalAction->setEnabled(enable);
    nextImageAction->setEnabled(enable);
    previousImageAction->setEnabled(enable);

    histDialogAction->setEnabled(enable);
    graphDialogAction->setEnabled(enable);
    selRectangleAction->setEnabled(enable);
    selFixedAction->setEnabled(enable);
    selHorizontalAction->setEnabled(enable);
    selVerticalAction->setEnabled(enable);
    selFullAction->setEnabled(enable);
    changeCursorColorAction->setEnabled(enable);

    detectCameraAction->setEnabled(true);
}
void MainWindow::saveAs()
{
    if (imageScene->IsNullImage()) {
        QMessageBox::information(this, "Information", "Nothing to save.");
        return;
    }
    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Image As ...");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        QFileInfo fi;
        fi.setFile(fileNames.at(0));
        QString ext = fi.suffix().toLower();
        if(ext == tr("png") || ext == tr("bmp")  || ext == tr("jpg")) {
            imageScene->SaveImage(fileNames.at(0));
        } else {
            QMessageBox::information(this, "Information", "Save error: bad format or filename.");
        }
    }
}
void MainWindow::zoomIn()
{
    if (imageScene->IsNullImage())
        return;

    imageView->zoomIn(3);
}

void MainWindow::zoomOut()
{
    if (imageScene->IsNullImage())
        return;

    imageView->zoomOut(3);
}

void MainWindow::fitWindow()
{
    if (imageScene->IsNullImage())
        return;

    //下記fitInViewでは、この後の拡大縮小の倍率がおかしくなる場合が有るので
    //imageView->fitInView(imageScene->sceneRect(), Qt::KeepAspectRatio);

    // 都度計算させる、Viewのm_zoom_levelも更新させるように
    int width = infoDock->isFloating() ?   this->width() - 10 : this->width() - 10 - infoDock->width();
    int height = this->height() - 100;

    double fRatio = imageScene->GetFitinRatio(width, height);
    imageView->fitWindow(fRatio);
}

void MainWindow::zoomOriginal()
{
    if (imageScene->IsNullImage())
        return;

    imageView->zoomOriginal();
}
void MainWindow::showPixelInfo(QString sInfo)
{
    imageInfoLabel->setText(sInfo);
}

void MainWindow::showDroppedImage(QList<QUrl> urllist)
{
    if (urllist.isEmpty())
        return;

    imageFilePathList.clear();
    imageIndex = -1;

    foreach (QUrl url, urllist) {
        QString sPath = url.toLocalFile();
        QString sEXT = QFileInfo(sPath).suffix().toUpper();
        if (sEXT == "BMP" || sEXT == "JPG" || sEXT == "PNG"){
            imageFilePathList.append(sPath);
            imageIndex++;
            showImage(sPath);
        }
    }
}

void MainWindow::showZoomRatio(qreal ratio)
{
    int percentage = (int)(ratio * 100);
    zoomInfoLabel->setText(QString::number(percentage) + "%");
    imageScene->zoomPercent = percentage;
}
void MainWindow::showPreviousImage()
{
    if (imageFilePathList.isEmpty())
        return;

    if (imageIndex == 0)
        return;

    imageIndex--;
    showImage(imageFilePathList[imageIndex]);
}

void MainWindow::showNextImage()
{
    if (imageFilePathList.isEmpty())
        return;

    if (imageIndex == imageFilePathList.size() - 1)
        return;

    imageIndex++;
    showImage(imageFilePathList[imageIndex]);
}
void MainWindow::showHistogramDialog()
{
    if (dialogHistoGram->IsShown())
        dialogHistoGram->hide();
    else
        dialogHistoGram->show();
}

void MainWindow::showGraphDialog()
{
    if (dialogGraph->IsShown())
        dialogGraph->hide();
    else
        dialogGraph->show();
}

void MainWindow::selRectangle()
{
    infoDock->ActivateDock(SELTYPE_RECT);
}

void MainWindow::selFixed()
{
    infoDock->ActivateDock(SELTYPE_FIX);
}

void MainWindow::selHorizontal()
{
    infoDock->ActivateDock(SELTYPE_HORZ);
}

void MainWindow::selVertical()
{
    infoDock->ActivateDock(SELTYPE_VERT);
}

void MainWindow::selFull()
{
    infoDock->ActivateDock(SELTYPE_FULL);
}

void MainWindow::changeCursorColor()
{
    if (imageScene->IsNullImage())
        return;

    QColorDialog dialog(imageScene->GetCursorColor(), this);

    QColor color = dialog.getColor();
    if (color.isValid())
        imageScene->SetCursorColor(color);
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About TegeViewer"),
        tr("ImageVierwer application with Qt.\n\n") +
        tr("Copyright (c) 2024 mengineer\n") +
        tr("This software is released under the MIT License."));
}

void MainWindow::showCameraImage()
{
    QPixmap pixmap;
    QVideoFrame frame = captureSession.videoSink()->videoFrame();
    if (frame.isValid()){
        pixmap = QPixmap::fromImage(frame.toImage());
        bool isSameSize;
        imageScene->SetPixmap(pixmap, &isSameSize);
    }
    if (!IsCameraStarted) {
        // Init View
        imageView->resetTransform();
        imageView->InitZoomLevel();
        imageView->setSceneRect(pixmap.rect());
        enabler(true);
        IsCameraStarted = true;
    }
}





