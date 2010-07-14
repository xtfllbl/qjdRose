#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rose=new QJDRose();
    cTable=new colorTable();
//    rose->resize(500,500);
//    resize(510,510);
    QGridLayout *gLayout=new QGridLayout();
    gLayout->addWidget(cTable,0,0);
    gLayout->addWidget(rose,0,1);
    ui->centralWidget->setLayout(gLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionZoomIn_triggered()  //放大
{
//    qDebug()<<rose->width()<<rose->height();
    rose->resize(rose->width()+100,rose->height()+100);
}

void MainWindow::on_actionZoomOut_triggered()  //缩小
{
//    qDebug()<<rose->width()<<rose->height();
    if(rose->width()>100 && rose->height()>100)  //小于100不缩小了
    {
        rose->resize(rose->width()-100,rose->height()-100);
    }
}
