#include "rightscale.h"
#include <QPainter>
#include <QDebug>
rightScale::rightScale(QWidget *parent) :
    QWidget(parent)
{
    setMaximumWidth(50);  //用不着这么宽
    setMinimumWidth(50);
    length=0;
    offset=0;
}

void rightScale::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawRect(1,1,width()-2,height()-2);

     diameter=(length*1.0/3)*2;   //使用float，最大限度减少误差
    painter.drawLine(5, offset, 5, int(diameter+offset));  // 对应圆圈的高度

    /// 刻度
    float longLine=diameter/6;
    for(int i=0;i<6;i++)
    {
        painter.drawLine(5, int(longLine*i+offset), 10, int(longLine*i+offset));
    }
    painter.drawLine(5, int(diameter+offset), 10, int(diameter+offset));  //手动保证最后一条线

    float shortLine=diameter/30;
    for(int i=0;i<30;i++)
    {
        painter.drawLine(5, int(shortLine*i+offset), 7, int(shortLine*i+offset));
    }

    paintPosLine(&painter);
}

void rightScale::setLength(int len,int off)
{
    length=len;
    offset=off;
    update();
}

void rightScale::setPosLine(int y)
{
    if(y>diameter+offset)
    {
        mouseY=int(diameter+offset);
    }
    if(y<offset)
    {
        mouseY=offset;
    }
    if(y>=offset && y<=(diameter+offset))
    {
        mouseY=y;
    }
    update();
}

void rightScale::paintPosLine(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(5,mouseY,15,mouseY);
}
