#ifndef MINMAXDIALOG_H
#define MINMAXDIALOG_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class MinMaxDialog;
}

class MinMaxDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MinMaxDialog(QWidget *parent = nullptr);
    ~MinMaxDialog();

    int XMin, XMax, YMin, YMax;
    // void SetMinMax(int xmin, int xmax, int ymin, int ymax);

private:
    Ui::MinMaxDialog *ui;
    void initUI();

    // QDialog interface
public slots:
    virtual void accept();
    virtual void reject();
    virtual int exec();
};

#endif // MINMAXDIALOG_H
