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
    QVector<qint64> originData;
    QVector<int> convetData;
    QVector<QVector<int> > colorData;

    /// 输入原始数据
    circleNumber=25;
    angleLineNumber=36;

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

    if(width()<=height())
    {
        /// 基本框架
        // 半径为长宽最小值的一半
        int radius=width()/3;  //有点太小了的味道
        painter.drawLine(offset,radius+offset,
                         radius*2+offset,radius+offset);
        painter.drawLine(radius+offset,offset,
                         radius+offset,radius*2+offset);

        QPoint a;
        a.setX(radius+offset);
        a.setY(radius+offset);
        painter.drawEllipse(a,radius,radius);

        // r网格
        //    int rUnit=width()/2/25;  // 分成25份
        //    int rUnitNum=width()/2/rUnit;
        //    for(int i=0;i<rUnitNum;i++)
        //    {
        //        painter.drawEllipse(a,rUnit*i,rUnit*i);
        //    }
        // 角度网格
        //    double kUnit=(PAI/18);  //弧度网格,10度一格
        //    for(int i=0;i<18;i++)
        //    {
        //        // 注意坐标系与现实y轴相反,所以理论计算正确，显示错误
        //        double x=width()/2*cos(kUnit*i);
        //        double y=width()/2*sin(kUnit*i);
        //        painter.drawLine(width()/2,width()/2,
        //                         width()/2+x,width()/2-y);
        //        painter.drawLine(width()/2,width()/2,
        //                         width()/2-x,width()/2+y);
        //    }

        //    for(int j=0;j<25;j++)
        //    {
        //        for(int i=0;i<18;i++)
        //        {
        ////            QBrush brush(QColor(j*10,j*10,i*10));  //目前颜色渐变
        //            QBrush brush(colorTable[50]);
        //            double x=(width()/2 -rUnit*j)*cos(kUnit*i);
        //            double y=(width()/2 -rUnit*j)*sin(kUnit*i);

        //            QPainterPath testPath1;
        //            testPath1.moveTo(width()/2, width()/2);
        //            /// 调节线段长
        //            testPath1.lineTo(width()/2+x,width()/2-y);
        //            /// 调节长度
        //            testPath1.arcTo(0+rUnit*j, 0+rUnit*j, (width()/2 -rUnit*j)*2, (width()/2 -rUnit*j)*2, i*10, 10.0);   //(x,y,width,height,startangle,sweaplength)
        //            testPath1.lineTo(width()/2,width()/2);
        //            painter.fillPath(testPath1,brush);

        //            /// --------------------------------------------------------------------------------------------------------------------------- ///
        //            QPainterPath testPath2;
        //            testPath2.moveTo(width()/2, width()/2);
        //            testPath2.lineTo(width()/2-x,width()/2+y);
        //            testPath2.arcTo(0+rUnit*j, 0+rUnit*j, (width()/2 -rUnit*j)*2, (width()/2 -rUnit*j)*2, 180+i*10, 10.0); //注意起始角度为180
        //            testPath2.lineTo(width()/2,width()/2);
        //            painter.fillPath(testPath2,brush);
        //        }
        //    }
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
