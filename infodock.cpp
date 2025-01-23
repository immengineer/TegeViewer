#include "infodock.h"
#include "ui_infodock.h"
#include "mainwindow.h"

InfoDock::InfoDock(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::InfoDock)
{
    ui->setupUi(this);
    Myparent = parentWidget();
    initUI();
    createActions();
}

InfoDock::~InfoDock()
{
    delete ui;
}

void InfoDock::ActivateDock(int roitype)
{
    ui->groupBoxData->setEnabled(true);
    ui->groupBoxRoiArea->setEnabled(true);
    ui->groupBoxRoiSel->setEnabled(true);
    ui->pushButtonCenter100->setEnabled(true);      // なぜか無効表示になるので
    checkRoiRadio(roitype);
}

int InfoDock::GetRoiType()
{
    return ui->buttonRoiGroup->checkedId();
}
void InfoDock::initUI()
{
    // set radio id >> roi selectionType
    ui->buttonRoiGroup->setId(ui->radioRect, SELTYPE_RECT);
    ui->buttonRoiGroup->setId(ui->radioFixedSize, SELTYPE_FIX);
    ui->buttonRoiGroup->setId(ui->radioHorizontal, SELTYPE_HORZ);
    ui->buttonRoiGroup->setId(ui->radioVertical, SELTYPE_VERT);
    ui->buttonRoiGroup->setId(ui->radioFull, SELTYPE_FULL);

    // QLabel vector
    aveLabels = QList<QLabel*> {ui->labelAveR, ui->labelAveG, ui->labelAveB};
    sdLabels = QList<QLabel*> {ui->labelSDevR, ui->labelSDevG, ui->labelSDevB};
    snLabels = QList<QLabel*> {ui->labelSNR, ui->labelSNG,ui->labelSNB};
    maxLabels = QList<QLabel*> {ui->labelMaxR, ui->labelMaxG, ui->labelMaxB};
    minLabels = QList<QLabel*> {ui->labelMinR, ui->labelMinG, ui->labelMinB};
    diffLabels = QList<QLabel*> {ui->labelDiffR, ui->labelDiffG, ui->labelDiffB};
}

void InfoDock::createActions()
{
    connect(ui->buttonRoiGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(changeRoiType(QAbstractButton*)));
    connect(ui->lineEditLeft, SIGNAL(editingFinished()), this, SLOT(changeFixedArea()));
    connect(ui->lineEditTop, SIGNAL(editingFinished()), this, SLOT(changeFixedArea()));
    connect(ui->lineEditWidth, SIGNAL(editingFinished()), this, SLOT(changeFixedArea()));
    connect(ui->lineEditHeight, SIGNAL(editingFinished()), this, SLOT(changeFixedArea()));
    connect(ui->pushButtonCopy, SIGNAL(clicked(bool)), this, SLOT(pushButtonCopy_clicked(bool)));
    connect(ui->pushButtonCenter100, SIGNAL(clicked(bool)), this, SLOT(setCenter100()));
}
void InfoDock::enabler()
{
    bool readOnly = ui->buttonRoiGroup->checkedId() != SELTYPE_FIX;
    QString backColor = readOnly ? "background-color: rgb(240, 240, 240)" : "background-color: rgb(255, 255, 255)";

    ui->lineEditLeft->setReadOnly(readOnly);
    ui->lineEditTop->setReadOnly(readOnly);
    ui->lineEditWidth->setReadOnly(readOnly);
    ui->lineEditHeight->setReadOnly(readOnly);

    ui->lineEditLeft->setStyleSheet(backColor);
    ui->lineEditTop->setStyleSheet(backColor);
    ui->lineEditWidth->setStyleSheet(backColor);
    ui->lineEditHeight->setStyleSheet(backColor);
}

void InfoDock::checkRoiRadio(int roiType)
{
    QAbstractButton* rb = ui->buttonRoiGroup->button(roiType);
    rb->setChecked(true);
    enabler();
    emit signalChangeRoiType(roiType);
}
void InfoDock::getAnalysisData(bool copy /*= false*/)
{
    analysisData data;

    // get pointer to MainWindow
    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);
    pMain->GetScene()->GetAnalysisData(&data);

    for (int color = 0; color < NUM_COLOR; ++color){
        if (data.pixels == 0){
            aveLabels[color]->setText("---.-");
            sdLabels[color]->setText("---.-");
            snLabels[color]->setText("---.-");
            maxLabels[color]->setText("---.-");
            minLabels[color]->setText("---.-");
            diffLabels[color]->setText("---.-");
        }
        else {
            aveLabels[color]->setText(QString::number(data.avrg[color], 'f', 2));
            sdLabels[color]->setText(QString::number(data.sd[color], 'f', 2));
            snLabels[color]->setText(QString::number(data.sn[color], 'f', 2));
            maxLabels[color]->setText(QString::number(data.maxVal[color]));
            minLabels[color]->setText(QString::number(data.minVal[color]));
            diffLabels[color]->setText(QString::number(data.diffVal[color]));
        }
    }
    QLocale locale;
    ui->labelTotalPixels->setText(locale.toString(data.pixels) + " pixels");

    if (copy){
        QClipboard *clipboard = QApplication::clipboard();
        QString s = "";
        for (int color = 0; color < NUM_COLOR; ++color) {
            s += aveLabels[color]->text() + TAB;
            s += sdLabels[color]->text() + TAB;
            s += snLabels[color]->text() + TAB;
            s += maxLabels[color]->text() + TAB;
            s += minLabels[color]->text() + TAB;
            s += diffLabels[color]->text() + TAB;
        }
        clipboard->setText(s.trimmed());
    }
}
void InfoDock::setCenter100()
{
    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);
    int left = (pMain->GetScene()->GetImageWidth() - 100) / 2;
    int top =   (pMain->GetScene()->GetImageHeight() - 100) / 2;   
    pMain->GetScene()->fixedArea.setRect(left, top, 100, 100);
    checkRoiRadio(SELTYPE_FIX);
}

void InfoDock::changeRoiType(QAbstractButton *button)
{
    emit signalChangeRoiType(ui->buttonRoiGroup->checkedId());
    enabler();
}

void InfoDock::changeFixedArea()
{
    MainWindow *pMain = qobject_cast<MainWindow*>(Myparent);
    pMain->GetScene()->fixedArea.setRect(ui->lineEditLeft->text().toInt(), ui->lineEditTop->text().toInt(),
                                         ui->lineEditWidth->text().toInt(), ui->lineEditHeight->text().toInt());

    emit signalChangeRoiType(ui->buttonRoiGroup->checkedId());
}

void InfoDock::updateRoiArea(QRect rect)
{
    ui->lineEditLeft->setText(QString::number(rect.left()));
    ui->lineEditTop->setText(QString::number(rect.top()));
    ui->lineEditWidth->setText(QString::number(rect.width()));
    ui->lineEditHeight->setText(QString::number(rect.height()));
    getAnalysisData();
}

void InfoDock::pushButtonCopy_clicked(bool checked)
{
    Q_UNUSED(checked);
    getAnalysisData(true);
}

