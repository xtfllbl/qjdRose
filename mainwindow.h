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
    QLabel *statsLabel1;

    int diffWid;
    int diffHei;
    int savedWid;
    int savedHei;
    bool isSmaller;
    bool isRecorded;
    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *);
private slots:
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void showData(int data);
};

#endif // MAINWINDOW_H
