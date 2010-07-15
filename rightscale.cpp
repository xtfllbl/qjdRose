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

    int radius=(length/3)*2;
    painter.drawLine(5,offset,5,radius+offset);  // 对应圆圈的高度

}

void rightScale::setLength(int len,int off)
{
//    qDebug()<<"QJDScale::setLength();"<<len;
    length=len;
    offset=off;
    update();
}
