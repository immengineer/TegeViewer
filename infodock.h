#ifndef INFODOCK_H
#define INFODOCK_H

#include <QDockWidget>
#include <QLabel>
#include <QList>
#include <QAbstractButton>
#include <QLocale>
#include "define.h"

namespace Ui {
class InfoDock;
}

class InfoDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit InfoDock(QWidget *parent = nullptr);
    ~InfoDock();
    void ActivateDock(int roitype);
    int GetRoiType();

signals:
    void signalChangeRoiType(int type);

public slots:
    void changeRoiType(QAbstractButton *button);    // Dialog内
    void changeFixedArea();
    void updateRoiArea(QRect rect);
    void getAnalysisData(bool copy = false);
    void setCenter100();

private slots:
    void pushButtonCopy_clicked(bool checked);

private:
    Ui::InfoDock *ui;
    QWidget* Myparent;

    void initUI();
    void createActions();
    void enabler();
    void checkRoiRadio(int roiType);

    QList<QLabel*> aveLabels;
    QList<QLabel*> sdLabels;
    QList<QLabel*> snLabels;
    QList<QLabel*> maxLabels;
    QList<QLabel*> minLabels;
    QList<QLabel*> diffLabels;

};

#endif // INFODOCK_H
