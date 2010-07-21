#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    rose=new QJDRose();
    rose->setPalette(Qt::white);  //仍然无用
    cTable=new colorTable();
    rScale=new rightScale();
    tScale=new topScale();

    statsLabel1=new QLabel();
    statsLabel1->setMinimumWidth(100);
    statsLabel1->setText("Let`s begin");
    ui->statusBar->addWidget(statsLabel1);
    rose->setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    setMouseTracking(true);

    connect(rose,SIGNAL(sigGetRange(int,int)),cTable,SLOT(setRange(int,int)));
    // 设置线段长度
    connect(rose,SIGNAL(sigGetLength(int,int)),rScale,SLOT(setLength(int,int)));
    connect(rose,SIGNAL(sigGetLength(int,int)),tScale,SLOT(setLength(int,int)));
    connect(rose,SIGNAL(sigGetLength(int,int)),cTable,SLOT(setLength(int,int)));
    // 设置偏移距
    connect(rose,SIGNAL(sigSetOffset(int,int)),rScale,SLOT(setOffset(int,int)));
    connect(rose,SIGNAL(sigSetOffset(int,int)),tScale,SLOT(setOffset(int,int)));
    // 链接鼠标
    connect(rose,SIGNAL(sigCurrentMousePosX(int)),tScale,SLOT(setPosLine(int)));
    connect(rose,SIGNAL(sigCurrentMousePosY(int)),rScale,SLOT(setPosLine(int)));
    rose->emitRange();  //链接之后发送信号

    // 链接显示数据
    // QPaintEngine::setSystemRect: Should not be changed while engine is active?????
    connect(rose,SIGNAL(sigCurrentData(int)),this,SLOT(showData(int)));  //为何会引发这种问题, 知道了，因为没有重绘。。。

    // 进行布局
    //    QGridLayout *gLayout=new QGridLayout();
    //    gLayout->addWidget(tScale,0,1);
    //    gLayout->addWidget(rScale,1,0);
    //    gLayout->addWidget(rose,1,1);
    //    QHBoxLayout *hLayout=new QHBoxLayout();
    //    hLayout->addWidget(cTable);
    //    hLayout->addLayout(gLayout);
    //    ui->centralWidget->setLayout(hLayout);  //设置layout

    QGridLayout *gLayout=new QGridLayout();
    gLayout->addWidget(tScale,0,2);
    gLayout->addWidget(rScale,1,1);
    gLayout->addWidget(rose,1,2);
    gLayout->addWidget(cTable,1,0);
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
    rose->getCurrentPosData();
    rose->resize(rose->width()+100,rose->height()+100);
//    rose->getCurrentPosData();  //竟然没有变化，稍后找你算帐
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
//        int roseWid=rose->width();
//        int roseHei=rose->height();
//        if(roseWid<roseHei)
//        {
//            roseWid=roseHei;
//        }
//        if(roseHei<roseWid)
//        {
//            roseHei=roseWid;
//        }

//        int allWid;
//        int allHei;
//        allWid=cTable->width()+rScale->width()+roseWid+50;
//        allHei=tScale->height()+roseHei+50;
//        qDebug()<<roseWid<<roseHei<<allWid<<allHei;
//        resize(allWid,allHei);   //一resize，就会降低到最小尺寸。。。为什么会强制缩小？
}

void MainWindow::mouseMoveEvent(QMouseEvent */*event*/)
{
//        qDebug()<<mapFromGlobal(event->pos());
}

void MainWindow::showData(int data)
{
    if(data!=-2)
    {
        statsLabel1->setText(QString::number(data));
    }
    if(data==-2)
    {
        statsLabel1->setText("Standby");
    }
}
