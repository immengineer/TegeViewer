#include "minmaxdialog.h"
#include "ui_minmaxdialog.h"

MinMaxDialog::MinMaxDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MinMaxDialog)
    , XMin(0)
    , XMax(0)
    , YMin(0)
    , YMax(0)
{
    ui->setupUi(this);
    initUI();
}

MinMaxDialog::~MinMaxDialog()
{
    delete ui;
}
void MinMaxDialog::initUI()
{
    ui->lineEditXMin->setFocus();
}

void MinMaxDialog::accept()
{
    XMin = ui->lineEditXMin->text().toInt();
    XMax = ui->lineEditXMax->text().toInt();
    YMin = ui->lineEditYMin->text().toInt();
    YMax = ui->lineEditYMax->text().toInt();
    QDialog::accept();
}

void MinMaxDialog::reject()
{
    QDialog::reject();
}

int MinMaxDialog::exec()
{
    ui->lineEditXMin->setText(QString::number(XMin));   ui->lineEditXMin->setInputMask("9999");
    ui->lineEditXMax->setText(QString::number(XMax));   ui->lineEditXMax->setInputMask("9999");
    ui->lineEditYMin->setText(QString::number(YMin));   ui->lineEditYMin->setInputMask("9999");
    ui->lineEditYMax->setText(QString::number(YMax));   ui->lineEditXMax->setInputMask("9999");
    return QDialog::exec();
}
