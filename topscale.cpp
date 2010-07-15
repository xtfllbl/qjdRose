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

    int radius=(length/3)*2;
    painter.drawLine(offset,5,radius+offset,5);  // 对应圆圈的长度
}

void topScale::setLength(int len,int off)
{
    length=len;
    offset=off;
    update();
}
