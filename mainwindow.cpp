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
    scale=new QJDScale();
    if(connect(rose,SIGNAL(sigGetRange(int,int)),cTable,SLOT(setRange(int,int))))
//        qDebug()<<"connect";   // 把数据连接上
    rose->emitRange();  //链接之后发送信号
    QGridLayout *gLayout=new QGridLayout();
    gLayout->addWidget(cTable,0,0);
    gLayout->addWidget(scale,0,1);
    gLayout->addWidget(rose,0,2);
//    qDebug()<<cTable->size()<<scale->size()<<rose->size();  // 640*480??
    ui->centralWidget->setLayout(gLayout);  //设置layout
    setCentralWidget(ui->centralWidget);  //设置显示
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
