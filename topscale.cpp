#include "topscale.h"
#include <QPainter>
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

    float radius=(length/3)*2;
    painter.drawLine(offset, 5, int(radius+offset), 5);  // 对应圆圈的长度

    /// 刻度
    float longLine=radius/6;
    for(int i=0;i<6;i++)
    {
        painter.drawLine(int(longLine*i+offset), 5, int(longLine*i+offset), 10 );
    }
    painter.drawLine(int(radius+offset),5,int(radius+offset),10);  //手动保证最后一条线

    float shortLine=radius/30;
    for(int i=0;i<30;i++)
    {
        painter.drawLine(int(shortLine*i+offset),5 , int(shortLine*i+offset), 7);
    }
}

void topScale::setLength(int len,int off)
{
    length=len;
    offset=off;
    update();
}
