#include "colortable.h"
#include <QDebug>
#include <QPainter>
colorTable::colorTable(QWidget *parent) :
        QWidget(parent)
{
    //会依据layout走偏
    setColorTable();
    minNum=100000;
    maxNum=0;
    length=0;
    offset=0;
    setMaximumWidth(100);
    setMinimumWidth(100);
}

void colorTable::setRange(int min, int max)
{
    // min==0,max==255
    minNum=min;
    maxNum=max;
}

void colorTable::paintEvent(QPaintEvent *)
{
    /// 需要设置和大小连动
    QPainter painter(this);
    painter.drawRect(1,1,width()-2,height()-2);

    /// 画色表
    // 最好和图形结合起来,起始位置什么的也都相同
    int colorValue;
    int wid=30;
    diameter=(length*1.0/3)*2;
    int hei=diameter;
    QImage imageTable(wid,hei,QImage::Format_ARGB32);
    imageTable.setColorTable(cTable);    //将颜色表附上

    for(int j=0;j<hei;j++)          //给每个像素上色
    {
        for(int i=0;i<wid;i++)
        {
            colorValue=255*j/hei;
            imageTable.setPixel(i,j,cTable[colorValue]);
        }
    }
    /// 刻度
    int startPos=0;
    if(maxNum>100000)  //这里将存在着一定空隙，不太美观
    {
        startPos=0;
    }
    if(maxNum>10000 && maxNum<100000)
    {
        startPos=10;
    }
    if(maxNum>1000 && maxNum<10000)
    {
        startPos=20;
    }
    if(maxNum>100 && maxNum<1000)
    {
        startPos=30;
    }
    if(maxNum<100)
    {
        startPos=40;
    }

    int startHeight=offset;         //将色表定在中间位置
    painter.drawImage(60,startHeight,imageTable);  // 定起始位置，画上去
    painter.drawRect(60,startHeight,wid,hei);        // 包围  drawRect ( int x, int y, int width, int height )
    for(int i=0;i<9;i++)
    {
        painter.drawLine(60,startHeight+hei/9*i,70,startHeight+hei/9*i);
        int number=maxNum/9*i;
        QString text=QString::number(number);
        painter.drawText(startPos,startHeight+hei/9*i+5,text);   // 依据数字的位数来改变刻度值的起始位置（想办法靠右）
    }
    QString text=QString::number(maxNum);
    painter.drawText(startPos,startHeight+hei,text);
}


void colorTable::setColorTable()
{
    cTable.clear();
    /*----------彩色色表----------*/
    int i,r,g,b;
    for (i = 0; i < 32; i++)
    {
        r = 0;
        g = 0;
        b = 131 + i * 4;
        cTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 32; i++)
    {
        r = 0;
        g = 4 + i * 4;
        b = 255;
        cTable<<qRgb(r,g,b);
    }
    for (i = 1; i < 32; i++)
    {
        r = 0;
        g = 127 + i * 4;
        b = 255;
        cTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 32; i++)
    {
        r = i * 4;
        g = 255;
        b = 255 - i * 4;
        cTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 32; i++)
    {
        r = 128 + i * 4;
        g = 255;
        b = 128 - i * 4;
        cTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 64; i++)
    {
        r = 255;
        g = 255 - i * 4;
        b = 0;
        cTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 33; i++)
    {
        r = 255 - i * 4;
        g = 0;
        b = 0;
        cTable<<qRgb(r,g,b);
    }
}

void colorTable::setLength(int len,int off)
{
//    qDebug()<<"color setLenth:: "<<len<<off;  //没有将这个和他们layout 在一起
    length=len;
    offset=off;
    update();       //及时改变
}

void colorTable::resizeWithCircle(int wid, int hei)
{
    resize(width(),hei);  //会地挡不住resizeEvent的诱惑主动投靠。。
    circleWidth=wid;
    circleHeight=hei;
}

void colorTable::resizeEvent(QResizeEvent *)
{
//    resize(width(),circleHeight);  //会地挡不住resizeEvent的诱惑主动投靠。。
}
