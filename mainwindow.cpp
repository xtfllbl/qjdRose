#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
// 简洁易用，只是调用的地方
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    counter=0;
    isRecorded=false;

    ui->setupUi(this);
    Qt::WindowFlags flags = 0;
    flags = Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
    setWindowFlags(flags); // 设置禁止最大化
    setFixedSize(600,537);  //这样一来，手动进行放大缩小也不可以

    rose=new QJDRose();
    offsetUnit=20;
    azimuthUnit=36;
    rose->setOffsetUnit(offsetUnit);
    rose->setAzimuthUnit(azimuthUnit);
    rose->start();

    cTable=new colorTable();
    rScale=new rightScale();
    tScale=new topScale();
    statsLabelFold=new QLabel();
    statsLabelFoldNum=new QLabel();
    statsLabelOffset=new QLabel();
    statsLabelOffsetNum=new QLabel();
    statsLabelAzimuth=new QLabel();
    statsLabelAzimuthNum=new QLabel();

    statsLabelFold->setMinimumWidth(30);
    statsLabelFoldNum->setMinimumWidth(80);
    statsLabelOffset->setMinimumWidth(50);
    statsLabelOffsetNum->setMinimumWidth(80);
    statsLabelAzimuth->setMinimumWidth(50);
    statsLabelAzimuthNum->setMinimumWidth(60);

    statsLabelFold->setText("Fold: ");
    statsLabelFoldNum->setText("Standby");
    statsLabelOffset->setText("Offset: ");
    statsLabelOffsetNum->setText("Standby");
    statsLabelAzimuth->setText("Azimuth: ");
    statsLabelAzimuthNum->setText("Standby");

    ui->statusBar->addWidget(statsLabelFold);
    ui->statusBar->addWidget(statsLabelFoldNum);
    ui->statusBar->addWidget(statsLabelOffset);
    ui->statusBar->addWidget(statsLabelOffsetNum);
    ui->statusBar->addWidget(statsLabelAzimuth);
    ui->statusBar->addWidget(statsLabelAzimuthNum);
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
    connect(rose,SIGNAL(sigCurrentMousePosX(int)),tScale,SLOT(setPosLine(int)));  //有时会更新不及时
    connect(rose,SIGNAL(sigCurrentMousePosY(int)),rScale,SLOT(setPosLine(int)));

    rose->emitRange();  //链接之后发送信号

    // 链接显示数据
    // QPaintEngine::setSystemRect: Should not be changed while engine is active?????
    connect(rose,SIGNAL(sigCurrentFold(int)),this,SLOT(showFoldData(int)));
    connect(rose,SIGNAL(sigCurrentAzimuth(int)),this,SLOT(showAzimuthData(int)));
    connect(rose,SIGNAL(sigCurrentOffset(int)),this,SLOT(showOffsetData(int)));

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
    // 有缺陷,不稳定因素太多
}

void MainWindow::mouseMoveEvent(QMouseEvent */*event*/)
{
    //        qDebug()<<mapFromGlobal(event->pos());
}

void MainWindow::showFoldData(int data)
{
    if(data!=-2)
    {
        statsLabelFoldNum->setText(QString::number(data));
    }
    if(data==-2)
    {
        statsLabelFoldNum->setText("Standby");
    }
}

void MainWindow::showOffsetData(int data)
{
    if(data!=-8000)
    {
        statsLabelOffsetNum->setText(QString::number(data));
    }
    if(data==-8000)
    {
        statsLabelOffsetNum->setText("Standby");
    }
}

void MainWindow::showAzimuthData(int data)
{
    if(data!=-2)
    {
        statsLabelAzimuthNum->setText(QString::number(data));
    }
    if(data==-2)
    {
        statsLabelAzimuthNum->setText("Standby");
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
    // 不显示小红线
    rScale->setPosLine(-2);
    tScale->setPosLine(-2);
    rose->innerCircle=-2;
    if(nowWid-roseWid+saveLen-50>=600 && nowHei-roseHei+saveLen-50>=537)  //不得小于最小值
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
    rScale->setPosLine(-2);
    tScale->setPosLine(-2);
    rose->innerCircle=-2;

    if(nowWid-roseWid+saveLen+50<=qApp->desktop()->screenGeometry().width()
        && nowHei-roseHei+saveLen+50<=qApp->desktop()->screenGeometry().height())  //不大于分辨率
        {
        setFixedSize(nowWid-roseWid+saveLen+50,nowHei-roseHei+saveLen+50);
    }
}

void MainWindow::on_actionClose_triggered()
{
    exit(0);
}

void MainWindow::on_actionChangeUnitNum_triggered()
{
    // just temp
    counter++;
    int plus=4;
    offsetUnit=10+counter*plus;
    azimuthUnit=12+counter*plus;
    rose->setOffsetUnit(offsetUnit);
    rose->setAzimuthUnit(azimuthUnit);
    rose->setData();
    rose->emitRange();  //改色表用
    update();
}
