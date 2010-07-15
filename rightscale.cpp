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

    float radius=(length*1.0/3)*2;   //使用float，最大限度减少误差
    painter.drawLine(5, offset, 5, int(radius+offset));  // 对应圆圈的高度

    /// 刻度
    float longLine=radius/6;
    for(int i=0;i<6;i++)
    {
        painter.drawLine(5, int(longLine*i+offset), 10, int(longLine*i+offset));
    }
    painter.drawLine(5, int(radius+offset), 10, int(radius+offset));  //手动保证最后一条线

    float shortLine=radius/30;
    for(int i=0;i<30;i++)
    {
        painter.drawLine(5, int(shortLine*i+offset), 7, int(shortLine*i+offset));
    }
}

void rightScale::setLength(int len,int off)
{
//    qDebug()<<"QJDScale::setLength();"<<len;
    length=len;
    offset=off;
    update();
}
