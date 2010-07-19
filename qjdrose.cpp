#include "qjdrose.h"
#include <QDebug>

#include "math.h"
#define PAI 3.1415926
// 在此类中手动做出玫瑰图
QJDRose::QJDRose(QWidget *)
{
    //    setMouseTracking(true);
    setPalette(Qt::white);  // 无用
    circleNumber=0;
    angleLineNumber=0;
    offset=0;
    setColorTable();
    setData();
    setMinimumSize(400,400);
    setMaximumSize(2000,2000);  //因为需要扩充，照顾layout
    //    qDebug()<<width()<<height();
}

void QJDRose::setData()
{
    /// 输入原始数据
    circleNumber=10;
    angleLineNumber=16;

    originData.resize(circleNumber*angleLineNumber);
    for(int i=0;i<originData.size();i++)
    {
        int ir = qrand()%100000;  //十万以内随机数
        originData[i]=ir;
    }
    /// 转换至色表区间
    maxNum=0;
    minNum=100000;
    cutNum=0;
    for(int i=0;i<originData.size();i++)
    {
        if(originData[i]>maxNum)
        {
            maxNum=originData[i];
        }
        if(originData[i]<minNum)
        {
            minNum=originData[i];
        }
    }
    qDebug()<<"emit"<<minNum<<maxNum;
    emit sigGetRange(minNum,maxNum);  //发出信号，让colorTable类能接受到,比connect还要早

    cutNum=int(  ceil( (maxNum-minNum)/255.0 )  );
    for(int i=0;i<originData.size();i++)
    {
        convetData<<int(floor(originData[i]*1.0/(cutNum+1)));   //此数不得大于255，否则程序崩溃
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
    //    qDebug()<<"setDataOut";
}

/// 需要调整一下画法，因为是从顶头开始并且顺时针，默认是逆时针，所以要取负值
void QJDRose::paintEvent(QPaintEvent *)
{
    //    qDebug()<<"QJDRose::paintEvent();";
    QPainter painter(this);
    painter.drawRect(1,1,width()-2,height()-2);

    length=0;
    if(width()<height())  /// 确保画出来的是圆而且不超出范围
    {
        length=width();
    }
    if(height()<width())
    {
        length=height();
    }
    if(width()==height())
    {
        length=width();
    }

    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿
    offset=length/8;  //偏移
    int more=10;        //适当添加一些余量
    radius=length/3;
//    qDebug()<<radius;
     kUnit=(2*PAI/angleLineNumber);  //整个圆除以需要分割的数量，得到每根斜线需要旋转的斜率
    qreal rUnit=radius*1.0/circleNumber;  //必须使用浮点，否则不准确
    int rUnitNum=int(radius/rUnit+1);  //加上最外圈,最外圈不再单独画

    emit sigGetLength(length,offset);  /// 发送相关信息

    circleMiddle.setX(radius+offset);
    circleMiddle.setY(radius+offset);

    /// 填充网格
    qreal turnAngleDegree;

    /// 从0度逆时针绘图
//    turnAngleDegree=360*1.0/angleLineNumber;     //逆时针
//    for(int i=0;i<circleNumber;i++)  //圈圈
//    {
//        for(int j=0;j<angleLineNumber;j++) //斜斜
//        {
//            QBrush brush(colorTable[ colorData[i][j] ]);   // colorData must <=255, otherwise pro will broken
//                        double x=(radius -rUnit*i)*cos(kUnit*j);
//                        double y=(radius -rUnit*i)*sin(kUnit*j);
//                        QPainterPath toFillPath;
//                        toFillPath.moveTo(radius+offset, radius+offset);  //移动到圆心
//                        toFillPath.lineTo(radius+offset+x,radius+offset-y);     // 调节线段长
//                        // arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
//                        toFillPath.arcTo(offset+rUnit*i, offset+rUnit*i, (radius -rUnit*i)*2, (radius -rUnit*i)*2,
//                                         j*turnAngleDegree, turnAngleDegree);  /// 此项注意要改长度和角度
//                        toFillPath.closeSubpath();
//                        painter.fillPath(toFillPath,brush);
//        }
//    }

    /// 从0度顺时针绘图
    //    turnAngleDegree=-360*1.0/angleLineNumber;     //顺时针
    //    for(int i=0;i<circleNumber;i++)  //圈圈
    //    {
    //        for(int j=0;j<angleLineNumber;j++) //斜斜
    //        {
    //            QBrush brush(colorTable[ colorData[i][j] ]);   // colorData must <=255, otherwise pro will broken
    //            double x=(radius -rUnit*i)*cos(kUnit*j);
    //            double y=(radius -rUnit*i)*sin(kUnit*j);
    //            QPainterPath toFillPath;
    //            toFillPath.moveTo(radius+offset, radius+offset);  //移动到圆心
    //            toFillPath.lineTo(radius+offset+x,radius+offset+y);     // 调节线段长
    //            // arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
    //            toFillPath.arcTo(offset+rUnit*i, offset+rUnit*i, (radius -rUnit*i)*2, (radius -rUnit*i)*2,
    //                             j*turnAngleDegree, turnAngleDegree);  /// 此项注意要改长度和角度
    //            toFillPath.closeSubpath();
    //            painter.fillPath(toFillPath,brush);
    //        }
    //    }

    /// 从0度顺时针绘图,success
    /// 在此，需要把所有的坐标保存下来，用于鼠标操作
    turnAngleDegree=-360*1.0/angleLineNumber;     //顺时针
    pointDataX.resize(circleNumber);
    pointDataY.resize(circleNumber);
    radiusData.resize(circleNumber);
    for(int i=0;i<circleNumber;i++)  //圈圈
    {
        pointDataX[i].resize(angleLineNumber);
        pointDataY[i].resize(angleLineNumber);
        radiusData[i]=radius -rUnit*i;  //记录半径
        for(int j=0;j<angleLineNumber;j++) //斜斜
        {
            QBrush brush(colorTable[ colorData[i][j] ]);   // colorData must <=255, otherwise pro will broken
            double x=(radius -rUnit*i)*cos(PAI/2+kUnit*j);
            double y=(radius -rUnit*i)*sin(PAI/2+kUnit*j);
            x=-x;  //x倒一下, y不要倒
            y=-y;
            QPainterPath toFillPath;
            toFillPath.moveTo(radius+offset, radius+offset);  //移动到圆心
            pointDataX[i][j]=int(radius+offset+x);  //保存记录
            pointDataY[i][j]=int(radius+offset+y);
            toFillPath.lineTo(radius+offset+x,radius+offset+y);     // 调节线段长,此项很重要，需要记录
            /// arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
            toFillPath.arcTo(offset+rUnit*i, offset+rUnit*i, (radius -rUnit*i)*2, (radius -rUnit*i)*2,
                             j*turnAngleDegree+90, turnAngleDegree);  /// 此项注意要改长度和角度
            toFillPath.closeSubpath();
//            painter.fillPath(toFillPath,brush);
        }
    }
    /// 基本框架
    // 半径为长宽最小值的一半
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);

    painter.drawLine(offset-more,radius+offset,
                     radius*2+offset+more,radius+offset);
    painter.drawLine(radius+offset,offset-more,
                     radius+offset,radius*2+offset+more);

    /// 一圈圈
    for(int i=0; i<rUnitNum; i++)
    {
        painter.drawEllipse(circleMiddle,rUnit*i,rUnit*i);
    }
    /// 一条条斜线,需要改变
    angleLineK.resize(angleLineNumber);  //存储斜率
    for(int i=0;i<angleLineNumber;i++)
    {
        double x=radius*cos(PAI/2+kUnit*i);
        double y=radius*sin(PAI/2+kUnit*i);
        x=-x;
        y=-y;
        painter.drawLine(int(radius+offset), int(radius+offset),
                         int(radius+offset+x), int(radius+offset+y));
        float k=y/x;
        if(angleLineNumber<1 && ((y/x)>50 || (y/x)<-50))
        {
            k=-1000;  //没用，还是无法判断
        }
        if(angleLineNumber>1 && ((y/x)>50 || (y/x)<-50))
        {
            k=1000;  //没用，还是无法判断
        }
        angleLineK[i]=k;
//        qDebug()<<i<<angleLineK[i]; //与人为认知是相反的，但是用鼠标计算一样是相反的，所以这里不做认知正确化处理
    }
    angleLineK<<1000;
    /// 写角度
    // 位置很难放
    QString angleText;
    QPointF writePos;
    /// 应当从90度开始写
    for(int i=0;i<angleLineNumber;i++)
    {
        double x=radius*cos(PAI/2+kUnit*i);
        double y=radius*sin(PAI/2+kUnit*i);
        x=-x;
        y=-y;

        float angleNum=360*1.0/angleLineNumber*i;  //先准确计算，然后模糊显示
        int showNum=int(angleNum);
        angleText=QString::number(showNum);
        writePos.setX(radius+offset+x);
        writePos.setY(radius+offset+y);

        QString zero="0";
        painter.drawText(radius+offset+2, offset-2, zero);
        if((angleNum>0 && angleNum<=90))
        {
            painter.drawText(int(writePos.x()+3), int(writePos.y()), angleText);
        }
        if (angleNum>90 && angleNum<=180)
        {
            painter.drawText(int(writePos.x()), int(writePos.y()+15), angleText);
        }
        if (angleNum>180 && angleNum<=270)
        {
            painter.drawText(int(writePos.x()-27), int(writePos.y()+15), angleText);  //适当调整
        }
        if (angleNum>270 && angleNum<=360)
        {
            painter.drawText(int(writePos.x()-30), int(writePos.y()), angleText);
        }
    }

    paintCurrentUnit(&painter);
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

void QJDRose::emitRange()
{
    emit sigGetRange(minNum,maxNum);
}

void QJDRose::resizeEvent(QResizeEvent *)
{
    /// 这个是增加了，但是主界面没有增加。。。
    int roseWid=width();
    int roseHei=height();
    if(roseWid<roseHei)
    {
        roseHei=roseWid;
    }
    if(roseHei<roseWid)
    {
        roseWid=roseHei;
    }
    resize(roseWid,roseHei);
}


void QJDRose::mouseMoveEvent(QMouseEvent *event)
{
    mouseX=event->pos().x();
    mouseY=event->pos().y();
    emit sigCurrentMousePosX(event->pos().x());
    emit sigCurrentMousePosY(event->pos().y());

//    nearX.clear();
//    nearY.clear();
//    for(int i=0;i<circleNumber;i++)  //从外向内
//    {
//        for(int j=0;j<angleLineNumber;j++)  //从90度开始顺时针
//        {
//            missX=mouseX-pointDataX[i][j];
//            missY=mouseY-pointDataY[i][j];
//            if(abs(missX)<10 && abs(missY)<10)
//            {
//                nearX<<pointDataX[i][j];
//                nearY<<pointDataY[i][j];
//                update();   // 每次都update不好意思，在有需要的时候update
//            }
//        }
//    }

    /// 还能根据鼠标当前的位置确定鼠标处于哪圈与哪圈的中间，然后将圈圈画出
    //圆心部分还没有照顾到
    mouseRadius=sqrt((mouseX-circleMiddle.x())*(mouseX-circleMiddle.x())
                     +(mouseY-circleMiddle.y())*(mouseY-circleMiddle.y()));
    for(int i=0;i<radiusData.size()-1;i++)
    {
        if(mouseRadius<radiusData[i] && mouseRadius>radiusData[i+1])
        {
            // 记录i和i+1的数据，然后画出来
            outerCircle=radiusData[i];
            innerCircle=radiusData[i+1];
        }
    }

    /// 判断mouse 是在哪两条斜线的中间
    int temp=angleLineNumber/4;
    int temp1;
    int temp2;
    bool is1=false;
    bool is2=false;
    bool is3=false;
    bool is4=false;
    if(mouseX>circleMiddle.x() && mouseY<circleMiddle.y())
    {
        qDebug()<<"1st";
        temp1=0;
        temp2=temp;
        is1=true;
    }
    if(mouseX>circleMiddle.x() && mouseY>circleMiddle.y())
    {
        qDebug()<<"2nd";
        temp1=temp;
        temp2=temp*2;
        is2=true;
    }
    if(mouseX<circleMiddle.x() && mouseY>circleMiddle.y())
    {
        qDebug()<<"3rd";
        temp1=temp*2;
        temp2=temp*3;
        is3=true;
    }
    if(mouseX<circleMiddle.x() && mouseY<circleMiddle.y())
    {
        qDebug()<<"4th";
        temp1=temp*3;
        temp2=temp*4;
        is4=true;
    }
    float mouseK;
    mouseK=(mouseY-circleMiddle.y())/(mouseX-circleMiddle.x());
    qDebug()<<"mouseK:: "<<mouseK;
//    for(int i=0;i<angleLineK.size();i++)
//    {
//        qDebug()<<i<<"::"<<angleLineK[i];
//    }

    /// 完全分开处理
    if(is1==true)
    {
        for(int i=temp1+1;i<=temp2;i++)
        {
            if(mouseK<angleLineK[i])
            {
                angleLine1=i-1;
                angleLine2=i;
                qDebug()<<angleLine1<<angleLine2;
                break;
            }
        }
    }
    if(is2==true)
    {
        for(int i=temp1+1;i<=temp2;i++)
        {
            if(mouseK<angleLineK[i])
            {
                angleLine1=i-1;
                angleLine2=i;
                break;
            }
        }
    }
    if(is3==true)
    {
        for(int i=temp1+1;i<=temp2;i++)
        {
            if(mouseK<angleLineK[i])
            {
                angleLine1=i-1;
                angleLine2=i;
                break;
            }
        }
    }
    if(is4==true)
    {
        // 真的要想办法在最后加一个斜率
        for(int i=temp1+1;i<=temp2;i++)
        {
//            qDebug()<<i<<":: "<<angleLineK[i];
            if(mouseK<angleLineK[i])
            {
                angleLine1=i-1;
                angleLine2=i;
                break;
            }
        }
    }
    update();  //don`t forget this
}

/// 获取当前鼠标的数据
void QJDRose::getCurrentPosData()
{
    // 由于是圆弧，所以不会非常准确
    qDebug()<<"pointDataX:: "<<pointDataX<<"\n"
            <<"pointDataY:: "<<pointDataY
            <<"~~~~~~~~~~";
}

/// 高亮当前鼠标滑过的模块
void QJDRose::paintCurrentUnit(QPainter *painter)
{
    QPen pen1;
    pen1.setColor(Qt::green);
    pen1.setWidth(3);
    painter->setPen(pen1);
    painter->drawEllipse(circleMiddle,outerCircle,outerCircle);
    painter->drawEllipse(circleMiddle,innerCircle,innerCircle);

    QPen pen2;
    pen2.setColor(Qt::red);
    pen2.setWidth(5);
    painter->setPen(pen2);
//    for(int i=0;i<nearX.size();i++)
//    {
//        painter->drawPoint(nearX[i],nearY[i]);
//    }
    // 知道当前最近的点，然后如何处理剩下的点呢？
    // 应当扩大范围，将所有临近点画出，然后判断
    painter->drawLine(int(radius+offset), int(radius+offset),
                     int(radius+offset-radius*cos(PAI/2+kUnit*angleLine1)), int(radius+offset-radius*sin(PAI/2+kUnit*angleLine1)));
    painter->drawLine(int(radius+offset), int(radius+offset),
                     int(radius+offset-radius*cos(PAI/2+kUnit*angleLine2)), int(radius+offset-radius*sin(PAI/2+kUnit*angleLine2)));

}
