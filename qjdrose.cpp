#include "qjdrose.h"
#include <QDebug>

#include "math.h"
#define PAI 3.1415926
// 在此类中手动做出玫瑰图
QJDRose::QJDRose(QWidget *)
{
    setPalette(Qt::white);
//    qDebug()<<width()<<height();
}

void QJDRose::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿
    painter.drawLine(0,height()/2,width(),height()/2);
    painter.drawLine(width()/2,0,width()/2,height());

    QPoint a;
    a.setX(width()/2);
    a.setY(height()/2);
    painter.drawEllipse(a,width()/2,height()/2);

    // r网格
    int rUnit=width()/2/25;  // 分成25份
    int rUnitNum=width()/2/rUnit;
    for(int i=0;i<rUnitNum;i++)
    {
        painter.drawEllipse(a,rUnit*i,rUnit*i);
    }
    // 角度网格
    double kUnit=(PAI/18);  //弧度网格,10度一格
    for(int i=0;i<18;i++)
    {
        // 注意坐标系与现实y轴相反,所以理论计算正确，显示错误
        double x=width()/2*cos(kUnit*i);
        double y=width()/2*sin(kUnit*i);
        painter.drawLine(width()/2,width()/2,
                         width()/2+x,width()/2-y);
        painter.drawLine(width()/2,width()/2,
                         width()/2-x,width()/2+y);
    }

    setColorTable();
    for(int j=0;j<25;j++)
    {
        for(int i=0;i<18;i++)
        {
//            QBrush brush(QColor(j*10,j*10,i*10));  //目前颜色渐变
            QBrush brush(colorTable[100]);
            double x=(width()/2 -rUnit*j)*cos(kUnit*i);
            double y=(width()/2 -rUnit*j)*sin(kUnit*i);

            QPainterPath testPath1;
            testPath1.moveTo(width()/2, width()/2);
            /// 调节线段长
            testPath1.lineTo(width()/2+x,width()/2-y);
            /// 调节长度
            testPath1.arcTo(0+rUnit*j, 0+rUnit*j, (width()/2 -rUnit*j)*2, (width()/2 -rUnit*j)*2, i*10, 10.0);   //(x,y,width,height,startangle,sweaplength)
            testPath1.lineTo(width()/2,width()/2);
            painter.fillPath(testPath1,brush);

            /// --------------------------------------------------------------------------------------------------------------------------- ///
            QPainterPath testPath2;
            testPath2.moveTo(width()/2, width()/2);
            testPath2.lineTo(width()/2-x,width()/2+y);
            testPath2.arcTo(0+rUnit*j, 0+rUnit*j, (width()/2 -rUnit*j)*2, (width()/2 -rUnit*j)*2, 180+i*10, 10.0); //注意起始角度为180
            testPath2.lineTo(width()/2,width()/2);
            painter.fillPath(testPath2,brush);
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
