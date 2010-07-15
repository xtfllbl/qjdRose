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
    rScale=new rightScale();
    tScale=new topScale();

    rose->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    setMouseTracking(true);

    connect(rose,SIGNAL(sigGetRange(int,int)),cTable,SLOT(setRange(int,int)));
    connect(rose,SIGNAL(sigGetLength(int,int)),rScale,SLOT(setLength(int,int)));
    connect(rose,SIGNAL(sigGetLength(int,int)),tScale,SLOT(setLength(int,int)));
    rose->emitRange();  //链接之后发送信号

    // 进行布局
    QGridLayout *gLayout=new QGridLayout();
    gLayout->addWidget(tScale,0,1);
    gLayout->addWidget(rScale,1,0);
    gLayout->addWidget(rose,1,1);
    QHBoxLayout *hLayout=new QHBoxLayout();
    hLayout->addWidget(cTable);
    hLayout->addLayout(gLayout);
    ui->centralWidget->setLayout(hLayout);  //设置layout
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

void MainWindow::resizeEvent(QResizeEvent *)
{
    /// resize含有诸多问题，无法实现
//    int roseWid=rose->width();
//    int roseHei=rose->height();
//    if(roseWid<roseHei)
//    {
//        roseWid=roseHei;
//    }
//    if(roseHei<roseWid)
//    {
//        roseHei=roseWid;
//    }

//    int allWid;
//    int allHei;
//    allWid=cTable->width()+rScale->width()+roseWid;
//    allHei=tScale->height()+roseHei;
//    qDebug()<<roseWid<<roseHei<<allWid<<allHei;
//    resize(allWid,allHei);   //一resize，就会降低到最小尺寸。。。为什么会强制缩小？
//    resize(width()+10,height()+10);  //同样不可取
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<event->pos();
}
