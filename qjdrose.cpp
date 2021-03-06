#include "qjdrose.h"
#include <QDebug>

#include "math.h"
#define PAI 3.1415926
// 在此类中手动做出玫瑰图
QJDRose::QJDRose(QWidget *)
{
    setPalette(Qt::white);
    circleNumber=0;
    angleLineNumber=0;
    offset=0;
    setColorTable();
    setData();
    //    qDebug()<<width()<<height();
}

void QJDRose::setData()
{
    /// 输入原始数据
    circleNumber=10;
    angleLineNumber=18;

    originData.resize(circleNumber*angleLineNumber);
    for(int i=0;i<originData.size();i++)
    {
        int ir = qrand();
        originData[i]=ir;
    }
    /// 转换至色表区间
    int maxNum=0;
    int cutNum=0;
    for(int i=0;i<originData.size();i++)
    {
        if(originData[i]>maxNum)
        {
            maxNum=originData[i];
        }
    }
    cutNum=ceil(maxNum/255.0);
    for(int i=0;i<originData.size();i++)
    {
        convetData<<originData[i]/cutNum;
    }

    /// 转换至二维数组用于显示
    colorData.resize(circleNumber);
    for(int i=0;i<circleNumber;i++)
    {
        colorData[i].resize(angleLineNumber);
        for(int j=0;j<angleLineNumber;j++)
        {
            colorData[i][j]=convetData[j+angleLineNumber*i];  //转化成二维
        }
    }
    //    qDebug()<<originData<<"\n"<<convetData<<"\n"<<colorData;
}

void QJDRose::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿
    offset=width()/8;  //偏移
    int more=10;        //适当添加一些余量
    int radius=width()/3;  //有点太小了的味道
    double kUnit=(2*PAI/angleLineNumber);  //整个圆除以需要分割的数量，得到每根斜线需要旋转的斜率
    qreal rUnit=radius*1.0/circleNumber;  //必须使用浮点，否则不准确
    int rUnitNum=radius/rUnit+1;  //加上最外圈,最外圈不再单独画

    QPointF a;  //圆心
    a.setX(radius+offset);
    a.setY(radius+offset);

    if(width()<=height())
    {
        /// 填充网格
        qreal turnAngleDegree;
        turnAngleDegree=360*1.0/angleLineNumber;     //旋转角度

        for(int i=0;i<circleNumber;i++)  //圈圈
        {
            for(int j=0;j<angleLineNumber;j++) //斜斜
            {
                QBrush brush(colorTable[ colorData[i][j] ]);
                double x=(radius -rUnit*i)*cos(kUnit*j);
                double y=(radius -rUnit*i)*sin(kUnit*j);

                QPainterPath toFillPath;
                toFillPath.moveTo(radius+offset, radius+offset);  //移动到圆心
                toFillPath.lineTo(radius+offset+x,radius+offset-y);     // 调节线段长, 这个有问题
                /// 此处非常奇怪，起始角度有问题, 旋转角度也许没问题
                // arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
                toFillPath.arcTo(offset+rUnit*i, offset+rUnit*i, (radius -rUnit*i)*2, (radius -rUnit*i)*2,
                                 j*turnAngleDegree, turnAngleDegree);  /// 此项注意要改长度和角度
                toFillPath.closeSubpath();
                painter.fillPath(toFillPath,brush);

//                QPen pen;
//                pen.setColor(Qt::blue);
//                painter.setPen(pen);
//                painter.drawPath(toFillPath);
                }
            }
        /// 基本框架
        // 半径为长宽最小值的一半
        painter.drawLine(offset-more,radius+offset,
                         radius*2+offset+more,radius+offset);
        painter.drawLine(radius+offset,offset-more,
                         radius+offset,radius*2+offset+more);

        /// 一圈圈
        for(int i=0; i<rUnitNum; i++)
        {
            painter.drawEllipse(a,rUnit*i,rUnit*i);
        }
        /// 一条条斜线
        for(int i=0;i<angleLineNumber;i++)
        {
            double x=radius*cos(kUnit*i);
            double y=radius*sin(kUnit*i);
            painter.drawLine(int(radius+offset), int(radius+offset),
                             int(radius+offset+x), int(radius+offset-y));
        }
    }
}

void QJDRose::setColorTable()
{
    colorTable.clear();
    /*----------彩色色表----------*/
    int i,r,g,b;
    for (i = 0; i < 32; i++)
    {
        r = 0;
        g = 0;
        b = 131 + i * 4;
        colorTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 32; i++)
    {
        r = 0;
        g = 4 + i * 4;
        b = 255;
        colorTable<<qRgb(r,g,b);
    }
    for (i = 1; i < 32; i++)
    {
        r = 0;
        g = 127 + i * 4;
        b = 255;
        colorTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 32; i++)
    {
        r = i * 4;
        g = 255;
        b = 255 - i * 4;
        colorTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 32; i++)
    {
        r = 128 + i * 4;
        g = 255;
        b = 128 - i * 4;
        colorTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 64; i++)
    {
        r = 255;
        g = 255 - i * 4;
        b = 0;
        colorTable<<qRgb(r,g,b);
    }
    for (i = 0; i < 33; i++)
    {
        r = 255 - i * 4;
        g = 0;
        b = 0;
        colorTable<<qRgb(r,g,b);
    }

}
