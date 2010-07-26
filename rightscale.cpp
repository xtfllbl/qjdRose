#include "rightscale.h"
#include <QPainter>
#include <QDebug>
rightScale::rightScale(QWidget *parent) :
    QWidget(parent)
{
    setMaximumWidth(70);  //用不着这么宽
    setMinimumWidth(70);
    radius=0;
    offset=0;
    minOffset=0;
    maxOffset=0;
    angle=-2;
}

void rightScale::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawRect(0,0,width()-2,height()-2);

    diameter=radius*2;   //使用float，最大限度减少误差
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
        if(i<0)
        {
            num=int(i*offsetUnit-minOffset);
//            qDebug()<<"<0::"<<num;
        }
        if(i>=0)
        {
            num=int(i*offsetUnit+minOffset);
//            qDebug()<<">0::"<<num;
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

    paintPosLine(&painter);  //这个想办法控制一下？  画那个小红线
}

void rightScale::setLength(int rad,int off)
{
//    qDebug()<<"right:: "<<len;
    radius=rad;
    offset=off;
    update();
}

void rightScale::setPosLine(int y)
{
    if(y>diameter+offset)  //大于范围
    {
        mouseY=-1;
    }
    if(y<offset)  //小于范围
    {
        mouseY=-1;
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
    if(mouseY!=-1)
    {
        painter->drawLine(5,mouseY,15,mouseY);
        // 可以根据这个来判断偏移，但是需要角度信息，这里需要大于45度角的时候的信息
        int tempOffset;

//        qDebug()<<"right:: "<<angle;
        if(angle!=-2)
        {
            if(  (angle>=0 && angle<=45)
                || (angle>=135 && angle<=225)
                || (angle>=315 && angle<=360))
                {
                if(mouseY>=offset && mouseY<=radius+offset)
                {
                    tempOffset=-(maxOffset-minOffset)*(radius-(mouseY-offset))/radius;
                }
                if(mouseY>=radius+offset && mouseY<=diameter+offset)
                {
                    tempOffset=(maxOffset-minOffset)*(mouseY-radius-offset)/radius;
                }
                emit sigCurrentOffset(tempOffset);
//                qDebug()<<"right:: "<<tempNum;
            }
        }
//        qDebug()<<tempNum;
    }
}

void rightScale::setOffset(float min, float max)
{
//    qDebug()<<"right setOffset";   //好像也没被执行
    minOffset=int(min);
    maxOffset=int(max);
}

void rightScale::resizeWithCircle(int , int )
{
//    resize(width(),hei);  //只需改变宽度
}

void rightScale::setAngle(int ang)
{
    angle=ang;
    update();
}
