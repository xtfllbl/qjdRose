#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qjdrose.h"
#include "colortable.h"
#include "rightscale.h"
#include "topscale.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QJDRose *rose;
    rightScale *rScale;
    topScale *tScale;
    colorTable *cTable;
private:
    Ui::MainWindow *ui;
    QLabel *statsLabelFold;
    QLabel *statsLabelFoldNum;
    QLabel *statsLabelOffset;
    QLabel *statsLabelOffsetNum;
    QLabel *statsLabelAzimuth;
    QLabel *statsLabelAzimuthNum;
    QWidget *change;

    int diffWid;
    int diffHei;
    int savedWid;
    int savedHei;
    bool isSmaller;
    bool isRecorded;
    int offsetUnit;
    int azimuthUnit;
    int counter;

    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *);
private slots:
    void on_actionChangeUnitNum_triggered();
    void on_actionClose_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void showFoldData(int data);
    void showOffsetData(int data);
    void showAzimuthData(int data);
};

#endif // MAINWINDOW_H
