#include "topscale.h"
#include <QPainter>
#include <QDebug>
topScale::topScale(QWidget *parent) :
    QWidget(parent)
{
    setMaximumHeight(50);  //用不着这么宽
    setMinimumHeight(50);
    radius=0;
    offset=0;
    minOffset=0;
    maxOffset=0;
    angle=-2;
}


void topScale::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawRect(0,0,width()-1,height()-1);

    diameter=radius*2;   /// 我擦，这是直径
    painter.drawLine(offset, 5, int(diameter+offset), 5);  // 对应圆圈的长度

    /// 刻度
    float longLine=diameter/6;
    float offsetUnit=(maxOffset-minOffset)/3;    //这个东西不准，乘回去误差太大
    //    qDebug()<<"top:: "<<maxOffset<<minOffset;
    QVector<QString> offsetText;
    int num;
    for(int i=-3;i<4;i++)
    {
        if(i<0)
        {
            num=int(i*offsetUnit-minOffset);
        }
        if(i>=0)
        {
            num=int(i*offsetUnit+minOffset);
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
        if(i==3)  //中间
        {
            painter.drawText(int(longLine*i+offset)-25, 25,
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

void topScale::setLength(int rad,int off)
{
    radius=rad;
    offset=off;
    update();
}

void topScale::setPosLine(int x)
{
    // 注意不要超出范围
    if(x>(diameter+offset))
    {
        mouseX=-1;
    }
    if(x<offset)
    {
        mouseX=-1;
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
    if(mouseX!=-1)
    {
        painter->drawLine(mouseX,5,mouseX,15);
        int tempOffset;
//        qDebug()<<"top:: "<<angle;
        if(angle!=-2 )
        {
            if((angle>45 && angle<135)
                || (angle>225 && angle<315))
                {
                if(mouseX>=offset && mouseX<=radius+offset)
                {
                    tempOffset=-(maxOffset-minOffset)*(radius-(mouseX-offset))/radius;
                }
                if(mouseX>=radius+offset && mouseX<=diameter+offset)
                {
                    tempOffset=(maxOffset-minOffset)*(mouseX-radius-offset)/radius;
                }
                emit sigCurrentOffset(tempOffset);
//                qDebug()<<"top:: "<<tempNum;
            }
        }
//        qDebug()<<tempNum;
    }
}

void topScale::setOffset(float min, float max)
{
//    qDebug()<<"top setOffset";
    minOffset=int(min);
    maxOffset=int(max);
}

void topScale::resizeWithCircle(int , int )
{
//    resize(wid,height());  //只需改变宽度
    /// 的确是更着走，但是界面不管，继续放大。。。。怎会如此无理
}

void topScale::setAngle(int ang)
{
    angle=ang;
    update();
}
