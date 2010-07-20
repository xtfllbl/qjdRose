#include "topscale.h"
#include <QPainter>
#include <QDebug>
topScale::topScale(QWidget *parent) :
    QWidget(parent)
{
    setMaximumHeight(50);  //用不着这么宽
    setMinimumHeight(50);
}


void topScale::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawRect(1,1,width()-2,height()-2);

     diameter=(length/3)*2;   /// 我擦，这是直径
    painter.drawLine(offset, 5, int(diameter+offset), 5);  // 对应圆圈的长度

    /// 刻度
    float longLine=diameter/6;
    for(int i=0;i<6;i++)
    {
        painter.drawLine(int(longLine*i+offset), 5, int(longLine*i+offset), 10 );
    }
    painter.drawLine(int(diameter+offset),5,int(diameter+offset),10);  //手动保证最后一条线

    float shortLine=diameter/30;
    for(int i=0;i<30;i++)
    {
        painter.drawLine(int(shortLine*i+offset),5 , int(shortLine*i+offset), 7);
    }

    paintPosLine(&painter);
}

void topScale::setLength(int len,int off)
{
    length=len;
    offset=off;
    update();
}

void topScale::setPosLine(int x)
{
    // 注意不要超出范围
    if(x>(diameter+offset))
    {
        mouseX=int(diameter+offset);
    }
    if(x<offset)
    {
        mouseX=offset;
    }
    if(x>=offset && x<=(diameter+offset))
    {
        mouseX=x;
    }
    update();
}

void topScale::paintPosLine(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(mouseX,5,mouseX,15);
}

void topScale::setOffset(int min, int max)
{
    minOffset=min;
    maxOffset=max;
}
