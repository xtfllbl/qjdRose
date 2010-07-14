#include "colortable.h"
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
    // 知道最大最小
    // min==0,max==255
    // 设置显示对应轴
    minNum=min;
    maxNum=max;
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
    //色表
    painter.drawImage(40,40,imageTable);

    //包围
    painter.drawRect(40,40,30,200);
    return;
    /// 刻度
//    for(int i=0;i<9;i++)
//    {
//        painter->drawLine(40,40+ui->mainToolBar->height()+ui->menuBar->height()+scrollarea->height()/9*i,
//                          50,40+ui->mainToolBar->height()+ui->menuBar->height()+scrollarea->height()/9*i);
//    }
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
