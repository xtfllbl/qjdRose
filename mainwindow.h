#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qjdrose.h"
#include "colortable.h"
#include "qjdscale.h"

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
    QJDScale *scale;
    colorTable *cTable;
private:
    Ui::MainWindow *ui;

private slots:
    void on_actionZoomOut_triggered();
    void on_actionZoomIn_triggered();
};

#endif // MAINWINDOW_H
