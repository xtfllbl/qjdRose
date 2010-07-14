#include "colortable.h"
#include <QDebug>
#include <QPainter>
colorTable::colorTable(QWidget *parent) :
    QWidget(parent)
{
    setColorTable();
    minNum=100000;
    maxNum=0;
    setMaximumWidth(100);
    setMinimumWidth(100);
}

void colorTable::setRange(int min, int max)
{
//  qDebug()<<"setRange();";
    // 知道最大最小
    // min==0,max==255
    // 设置显示对应轴
    minNum=min;
    maxNum=max;
//    qDebug()<<minNum<<maxNum;
}

void colorTable::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    /// 画色表
    QImage imageTable(30,200,QImage::Format_ARGB32);
    int colorValue;
    imageTable.setColorTable(cTable);    //将颜色表附上

    for(int j=0;j<200;j++)          //给每个像素上色
    {
        for(int i=0;i<30;i++)
        {
            colorValue=255*j/200;
            imageTable.setPixel(i,j,cTable[colorValue]);
        }
    }
    painter.drawImage(60,40,imageTable);  // 画上去
    painter.drawRect(60,40,30,200);        // 包围  drawRect ( int x, int y, int width, int height )
    /// 刻度
    int startPos=0;
    if(maxNum>100000)
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
//    qDebug()<<startPos;
    for(int i=0;i<9;i++)
    {
        painter.drawLine(60,40+200/9*i,70,40+200/9*i);
        int number=maxNum/9*i;
        QString text=QString::number(number);
        painter.drawText(startPos,40+200/9*i+5,text);   // 依据数字的位数来改变刻度值的起始位置（想办法靠右）
    }
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
