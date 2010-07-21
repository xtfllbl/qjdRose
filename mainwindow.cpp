#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    isRecorded=false;

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
    // 界面同步缩放  not needed ant more
    //    connect(rose,SIGNAL(sigWidgetSize(int,int)),tScale,SLOT(resizeWithCircle(int,int)));
    //    connect(rose,SIGNAL(sigWidgetSize(int,int)),rScale,SLOT(resizeWithCircle(int,int)));
    //    connect(rose,SIGNAL(sigWidgetSize(int,int)),cTable,SLOT(resizeWithCircle(int,int)));
    rose->emitRange();  //链接之后发送信号

    // 链接显示数据
    // QPaintEngine::setSystemRect: Should not be changed while engine is active?????
    connect(rose,SIGNAL(sigCurrentData(int)),this,SLOT(showData(int)));  //为何会引发这种问题, 知道了，因为没有重绘。。。

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
//    rose->getCurrentPosData();
//    rose->resize(rose->width()+100,rose->height()+100);
}

void MainWindow::on_actionZoomOut_triggered()  //缩小
{
//    if(rose->width()>100 && rose->height()>100)  //小于100不缩小了
//    {
//        rose->resize(rose->width()-100,rose->height()-100);
//    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    /// resize含有诸多问题，无法实现
    /// 主要是不能小于总体尺寸，否则就会不给力。。。
    int roseWid=rose->width();
    int roseHei=rose->height();
    if(roseWid<roseHei)
    {
        roseWid=roseHei;
    }
    if(roseHei<roseWid)
    {
        roseHei=roseWid;
    }

    /// 终于勉强实现了这一个效果，但是仍然存在着问题
    // 在左右上下单方面拖拉缩小将会不起作用，需要斜向缩小才能实现，问题未知
    if(isRecorded==false)
    {
        diffWid=width()-roseWid;  //只想记录一次，但是resize的第一次的值总是不正确的
        diffHei=height()-roseHei;
        if(diffWid>0 && diffHei>0)
        {
            isRecorded=true;
        }
        else
        {
            isRecorded=false;
        }
    }

    qDebug()<<roseWid<<roseHei<<diffWid<<diffHei;
    /// 没有任何变化，因为的确没有变化，需要一个固定数值
    resize(diffWid+roseWid, diffHei+roseHei);
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
