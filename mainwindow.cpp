#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    isRecorded=false;

    ui->setupUi(this);
    Qt::WindowFlags flags = 0;
    flags = Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
    setWindowFlags(flags); // 设置禁止最大化
    setFixedSize(600,537);  //这样一来，手动进行放大缩小也不可以

    rose=new QJDRose();
    //    rose->setPalette(Qt::white);  //仍然无用
    rose->setOffsetUnit(20);
    rose->setAzimuthUnit(36);
    rose->start();

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
    connect(rose,SIGNAL(sigSetOffset(float,float)),rScale,SLOT(setOffset(float,float)));
    connect(rose,SIGNAL(sigSetOffset(float,float)),tScale,SLOT(setOffset(float,float)));
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

void MainWindow::resizeEvent(QResizeEvent *)
{
    /// 通过手动放大缩小并且直接固定来控制大小。。。
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

/// 缩小
void MainWindow::on_actionZoomOut_triggered()
{
    int roseWid=rose->width();
    int roseHei=rose->height();
    int saveLen;
    if(roseWid<roseHei)
        saveLen=roseWid;
    else
        saveLen=roseHei;

    int nowWid=width();
    int nowHei=height();
//    resize(nowWid-roseWid+saveLen-50,nowHei-roseHei+saveLen-50);
    if(nowWid-roseWid+saveLen-50>=600 && nowHei-roseHei+saveLen-50>=537)
    {
        setFixedSize(nowWid-roseWid+saveLen-50,nowHei-roseHei+saveLen-50);
    }
}

/// 放大
void MainWindow::on_actionZoomIn_triggered()
{
    int roseWid=rose->width();
    int roseHei=rose->height();
    int saveLen;
    if(roseWid<roseHei)
        saveLen=roseWid;
    else
        saveLen=roseHei;

    int nowWid=width();
    int nowHei=height();
//    resize(nowWid-roseWid+saveLen+50,nowHei-roseHei+saveLen+50);
    setFixedSize(nowWid-roseWid+saveLen+50,nowHei-roseHei+saveLen+50);
}

void MainWindow::on_actionClose_triggered()
{
    exit(0);
}
