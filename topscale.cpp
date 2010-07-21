#include "topscale.h"
#include <QPainter>
#include <QDebug>
topScale::topScale(QWidget *parent) :
    QWidget(parent)
{
    setMaximumHeight(50);  //用不着这么宽
    setMinimumHeight(50);
    length=0;
    offset=0;
    minOffset=0;
    maxOffset=0;
}


void topScale::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawRect(1,1,width()-2,height()-2);

     diameter=(length/3)*2;   /// 我擦，这是直径
    painter.drawLine(offset, 5, int(diameter+offset), 5);  // 对应圆圈的长度

    /// 刻度
    float longLine=diameter/6;
    float offsetUnit=(maxOffset-minOffset)/3;    //这个东西不准，乘回去误差太大
    //    qDebug()<<"top:: "<<maxOffset<<minOffset;
    QVector<QString> offsetText;
    int num;
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
        painter.drawLine(int(longLine*i+offset), 5, int(longLine*i+offset), 10 );
        if(i!=3)
        {
            painter.drawText(int(longLine*i+offset)-20, 25, offsetText[i]);
        }
        if(i==3)
        {
            painter.drawText(int(longLine*i+offset)-20, 40,
                             QString::number(minOffset)+"("+QString::number(-minOffset)+")");  //这个很难办,位置很难放
        }
    }
    painter.drawLine(int(diameter+offset),5,int(diameter+offset),10);
    painter.drawText( int(diameter+offset)-20, 25, offsetText[6]);

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
//    qDebug()<<"top setOffset";
    minOffset=min;
    maxOffset=max;
}
