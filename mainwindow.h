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

    void resizeEvent(QResizeEvent *);
private slots:
    void on_actionZoomOut_triggered();
    void on_actionZoomIn_triggered();
};

#endif // MAINWINDOW_H
