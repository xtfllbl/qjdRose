#include "rightscale.h"
#include <QPainter>
#include <QDebug>
rightScale::rightScale(QWidget *parent) :
    QWidget(parent)
{
    setMaximumWidth(70);  //用不着这么宽
    setMinimumWidth(70);
    length=0;
    offset=0;
    minOffset=0;
    maxOffset=0;
}

void rightScale::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawRect(1,1,width()-2,height()-2);

     diameter=(length*1.0/3)*2;   //使用float，最大限度减少误差
    painter.drawLine(5, offset, 5, int(diameter+offset));  // 对应圆圈的高度

    /// 刻度 & 坐标
    float longLine=diameter/6;
    float offsetUnit=(maxOffset-minOffset)/3;
//    qDebug()<<"right:: "<<maxOffset<<minOffset;
    QVector<QString> offsetText;
    int num;
    // 。。。真的有最小offset存在则坐标不能这样写了
    for(int i=-3;i<4;i++)
    {
        if(i<3)
        {
            num=i*offsetUnit-minOffset;
        }
        if(i>=3)
        {
            num=i*offsetUnit+minOffset;
        }
        offsetText<<QString::number(num);
    }
    for(int i=0;i<6;i++)
    {
        painter.drawLine(5, int(longLine*i+offset), 10, int(longLine*i+offset));
        // 想想如何谢坐标轴,we have min/maxOffset
        if(i!=3)
        {
        painter.drawText(13, int(longLine*i+offset)+5, offsetText[i]);
    }
        if(i==3)
        {
            painter.drawText(13, int(longLine*i+offset)+5,
                             QString::number(minOffset)+"("+QString::number(-minOffset)+")");
        }
    }
    painter.drawLine(5, int(diameter+offset), 10, int(diameter+offset));  //手动保证最后一条线
    painter.drawText( 13, int(diameter+offset)+5, offsetText[6]);  //手动保证最后一条线

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

// 画鼠标对应的刻度
void rightScale::paintPosLine(QPainter *painter)
{
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(5,mouseY,15,mouseY);
}

void rightScale::setOffset(int min, int max)
{
//    qDebug()<<"right setOffset";   //好像也没被执行
    minOffset=min;
    maxOffset=max;
}
