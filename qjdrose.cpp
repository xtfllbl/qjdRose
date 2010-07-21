#include "qjdrose.h"
#include <QDebug>

#include "math.h"
#define PAI 3.1415926
#define CIRCLE 15
#define ANGLE 16   //这个一大会出问题？？
// 在此类中手动做出玫瑰图
QJDRose::QJDRose(QWidget *)
{
    //    setMouseTracking(true);
    setPalette(Qt::white);  // 无用
    circleNumber=0;
    angleLineNumber=0;
    offset=0;
    innerCircle=-2;

    originUnitData.resize(CIRCLE);
    for(int i=0;i<CIRCLE;i++)
        originUnitData[i].resize(ANGLE);
    for(int i=0;i<CIRCLE;i++)
        for(int j=0;j<ANGLE;j++)
            originUnitData[i][j]=0;

    colorUnitData.resize(CIRCLE);
    for(int i=0;i<CIRCLE;i++)
        colorUnitData[i].resize(ANGLE);
    for(int i=0;i<CIRCLE;i++)
        for(int j=0;j<ANGLE;j++)
            colorUnitData[i][j]=0;

    setColorTable();
    setOaData();
    setData();
    setMinimumSize(400,400);
    setMaximumSize(2000,2000);  //因为需要扩充，照顾layout
}

void QJDRose::setOaData()
{
    QFile file;
    file.setFileName("/home/xtf/gqx-all-new.oa");
    if(!file.open(QFile::ReadOnly))
        qDebug()<<"open failed";
    qint64 fileSize;
    qint64 oaNum;
    fileSize=file.size();
    oaNum=(file.size()-200)/20;
    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);   //使用readRawData时可无视
    in.skipRawData(200);

    minOffset=100000;
    minAzimuth=100000;
    maxOffset=0;
    maxAzimuth=0;
    /// 录入数据，兼顾求最大最小
    for(int i=0;i<oaNum;i++)
    {
        in>>oa;  // warning::must use qt4.5, 高版本将会产生错误
        offsetData<<oa.offset;
        azimuthData<<oa.azimuth;

        if(oa.offset<minOffset)
        {
            minOffset=oa.offset;
        }
        if(oa.offset>maxOffset)
        {
            maxOffset=oa.offset;
        }
        if(oa.azimuth<minAzimuth)
        {
            minAzimuth=oa.azimuth;
        }
        if(oa.azimuth>maxAzimuth)
        {
            maxAzimuth=oa.azimuth;
        }
    }
    qDebug()<<"rose ::"<<minOffset<<maxOffset;
    file.close();
}

void QJDRose::setData()
{
    circleNumber=CIRCLE;    //10
    angleLineNumber=ANGLE;    //16

    /// 新加处理
    // -------------------归类-------------------- //
    // 网格划分
    float offsetUnitLength;
    float azimuthUnitLength;
    int gridX;
    int gridY;

    offsetUnitLength=(maxOffset-minOffset)/circleNumber;
    azimuthUnitLength=(maxAzimuth-minAzimuth)/angleLineNumber;
    for(int i=0;i<offsetData.size();i++)
    {
        gridX=ceil((offsetData[i]-minOffset)/offsetUnitLength)-1;
        if(gridX>10)
            gridX=10;
        if(gridX<0)
            gridX=0;
        gridY=ceil((azimuthData[i]-minAzimuth)/azimuthUnitLength)-1;
        if(gridY>15)
            gridY=15;
        if(gridY<0)
            gridY=0;

        originUnitData[gridX][gridY]++;
    }

    /// 找到转换后数字的最大
     maxOriginUnit=0;
     minOriginUnit=1000000;
    int cutNum=0;
    for(int i=0;i<circleNumber;i++)
    {
        for(int j=0;j<angleLineNumber;j++)
        {
            if(originUnitData[i][j]>maxOriginUnit)
            {
                maxOriginUnit=originUnitData[i][j];
            }
            if(originUnitData[i][j]<minOriginUnit)
            {
                minOriginUnit=originUnitData[i][j];
            }
        }
    }
//    emit sigGetRange(minOriginUnit,maxOriginUnit);  //发出信号，让colorTable类能接受到,比connect还要早

    cutNum=ceil(maxOriginUnit/255);
    for(int i=0;i<circleNumber;i++)
    {
        for(int j=0;j<angleLineNumber;j++)
        {
            if(originUnitData[i][j]!=0)
            {
                //不许大于255
                colorUnitData[i][j]=ceil(originUnitData[i][j]/(cutNum+1));
            }
        }
    }
    qDebug()<<originUnitData;
    qDebug()<<"cutNum:: "<<cutNum;
    qDebug()<<colorUnitData;
}

// 废弃，原始随机数据产生方法
void QJDRose::setData2()
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
    //    qDebug()<<"emit"<<minNum<<maxNum;
    emit sigGetRange(minNum,maxNum);  //发出信号，让colorTable类能接受到,比connect还要早

    cutNum=int(  ceil( (maxNum-minNum)/255.0 )  );
    for(int i=0;i<originData.size();i++)
    {
        convetData<<int(floor(originData[i]*1.0/(cutNum+1)));   //此数不得大于255，否则程序崩溃
    }

    /// 转换至二维数组用于提取数据,必须与显示的二维数组同步
    // 此数组用于鼠标移动时显示相关的数据
    originDataDouble.resize(circleNumber);
    for(int i=0;i<circleNumber;i++)
    {
        originDataDouble[i].resize(angleLineNumber);
        for(int j=0;j<angleLineNumber;j++)
        {
            originDataDouble[i][j]=originData[j+angleLineNumber*i];
        }
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
}

/// 需要调整一下画法，因为是从顶头开始并且顺时针，默认是逆时针，所以要取负值
void QJDRose::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿
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

    offset=length/8;  //偏移
    int more=10;        //适当添加一些余量
    radius=length/3;
    kUnit=(2*PAI/angleLineNumber);  //整个圆除以需要分割的数量，得到每根斜线需要旋转的斜率
    rUnit=radius*1.0/circleNumber;  //必须使用浮点，否则不准确
    int rUnitNum=int(radius/rUnit+1);  //加上最外圈,最外圈不再单独画

    emit sigGetLength(length,offset);  /// 发送相关信息,还不如直接给半径来的直接
    emit sigSetOffset(minOffset,maxOffset);

    circleMiddle.setX(radius+offset);
    circleMiddle.setY(radius+offset);

    /// 填充网格
    /// 从0度顺时针绘图,success
    /// 在此，需要把所有的坐标保存下来，用于鼠标操作
    turnAngleDegree=-360*1.0/angleLineNumber;     //顺时针
    pointDataX.resize(circleNumber);
    pointDataY.resize(circleNumber);
    radiusData.resize(circleNumber);
    minRadius=1000000;
    maxRadius=0;
    for(int i=0;i<circleNumber;i++)  //圈圈
    {
        pointDataX[i].resize(angleLineNumber);
        pointDataY[i].resize(angleLineNumber);
        radiusData[i]=int(radius -rUnit*i);  //记录半径
        if(radiusData[i]<minRadius)
        {
            minRadius=radiusData[i];  //记录半径最小值
            minRadiusID=i;
        }
        if(radiusData[i]>maxRadius)
        {
            maxRadius=radiusData[i];  //记录半径最小值
            maxRadiusID=i;
        }
        for(int j=0;j<angleLineNumber;j++) //斜斜
        {
            QBrush brush(colorTable[ colorUnitData[i][j] ]);   // colorData must <=255, otherwise pro will broken
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
            painter.fillPath(toFillPath,brush);
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
            k=-1000;
        }
        if(angleLineNumber>1 && ((y/x)>50 || (y/x)<-50))
        {
            k=1000;
        }
        angleLineK[i]=k;
    }
    angleLineK<<1000;  // 在最后加上无穷大，以便判断
    /// 写角度
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

    /// 发送相关信息，显示当前的数据情况
    int iii;
    int jjj;
    iii=outerCircleID;
    jjj=angleLine1ID;
    if(innerCircle!=-2)
    {
        emit sigCurrentData(originUnitData[iii][jjj]);
    }
    if(innerCircle==-2)
    {
        emit sigCurrentData(-2);
    }
    //    qDebug()<<circleNumber<<angleLineNumber;(10,16)
    //        qDebug()<<"circle:: "<<innerCircleID<<outerCircleID;  //inner(1~9) outer(0~9)  使用outerCircleID
    //    qDebug()<<"angleLine:: "<<angleLine1ID<<angleLine2ID;  //正常，请使用angleLine1ID(0~15)  angleLine2ID(1~16)
    /// 如果鼠标不在范围内应该是不显示的
    if(innerCircle!=-2)
    {
        paintCurrentUnit(&painter);  //由于不显示，所以无法在此函数中发送出界信息
    }
}

void QJDRose::setColorTable()
{
    colorTable.clear();
    /*----------彩色色表----------*/
    colorTable<<qRgb(255,255,255);
    int i,r,g,b;
    for (i = 1; i < 32; i++)
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
    emit sigGetRange(minOriginUnit,maxOriginUnit);
}

void QJDRose::resizeEvent(QResizeEvent *)
{
    /// 这个是增加了，但是主界面没有增加。。。
    // 考虑与其他widget进行同步，主界面放弃教育
//    int roseWid=width();
//    int roseHei=height();
//    if(roseWid<roseHei)
//    {
//        roseHei=roseWid;
//    }
//    if(roseHei<roseWid)
//    {
//        roseWid=roseHei;
//    }
//    emit sigWidgetSize(roseWid,roseHei);     //发送长宽，用于resize其他控件，其余的不用写在resizeEvent中，单单resize就可以了
//    resize(roseWid,roseHei);
}


void QJDRose::mouseMoveEvent(QMouseEvent *event)
{
    mouseX=event->pos().x();
    mouseY=event->pos().y();
    emit sigCurrentMousePosX(event->pos().x());
    emit sigCurrentMousePosY(event->pos().y());

    /// ----------还能根据鼠标当前的位置确定鼠标处于哪圈与哪圈的中间，然后将圈圈画出----------///
    mouseRadius=sqrt((mouseX-circleMiddle.x())*(mouseX-circleMiddle.x())
                     +(mouseY-circleMiddle.y())*(mouseY-circleMiddle.y()));
    // 超出范围则不显示,very good
    if(mouseRadius>maxRadius)
    {
        innerCircle=-2;
    }
    for(int i=0;i<radiusData.size()-1;i++)
    {
        if(mouseRadius<radiusData[i] && mouseRadius>radiusData[i+1])
        {
            // 记录i和i+1的数据，然后画出来
            outerCircle=radiusData[i];
            innerCircle=radiusData[i+1];
            outerCircleID=i;
            innerCircleID=i+1;
        }
    }
    //圆心部分
    if(mouseRadius<minRadius)
    {
        outerCircle=minRadius;
        outerCircleID=minRadiusID;
        innerCircle=-1;
    }

    /// ----------------------判断mouse 是在哪两条斜线的中间-------------------- ///
    // 此处做的太过冗余，考虑精简
    int temp=angleLineNumber/4;
    int temp1;
    int temp2;
    bool is1=false;
    bool is2=false;
    bool is3=false;
    bool is4=false;
    if(mouseX>circleMiddle.x() && mouseY<circleMiddle.y())
    {
        temp1=0;
        temp2=temp;
        is1=true;
    }
    if(mouseX>circleMiddle.x() && mouseY>circleMiddle.y())
    {
        temp1=temp;
        temp2=temp*2;
        is2=true;
    }
    if(mouseX<circleMiddle.x() && mouseY>circleMiddle.y())
    {
        temp1=temp*2;
        temp2=temp*3;
        is3=true;
    }
    if(mouseX<circleMiddle.x() && mouseY<circleMiddle.y())
    {
        temp1=temp*3;
        temp2=temp*4;
        is4=true;
    }
    float mouseK;
    mouseK=(mouseY-circleMiddle.y())/(mouseX-circleMiddle.x());
    //    qDebug()<<"mouseK:: "<<mouseK;

    /// 完全分开处理
    if(is1==true)
    {
        for(int i=temp1+1;i<=temp2;i++)
        {
            if(mouseK<angleLineK[i])
            {
                angleLine1=i-1;
                angleLine2=i;
                angleLine1ID=i-1;
                angleLine2ID=i;
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
                angleLine1ID=i-1;
                angleLine2ID=i;
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
                angleLine1ID=i-1;
                angleLine2ID=i;
                break;
            }
        }
    }
    if(is4==true)
    {
        // 真的要想办法在最后加一个斜率
        for(int i=temp1+1;i<=temp2;i++)
        {
            //qDebug()<<i<<":: "<<angleLineK[i];
            if(mouseK<angleLineK[i])
            {
                angleLine1=i-1;
                angleLine2=i;
                angleLine1ID=i-1;
                angleLine2ID=i;
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
    /// 现在是判断出所在斜线和圆弧区间内
    //    QPen pen1;
    //    pen1.setColor(Qt::white);
    //    pen1.setWidth(3);
    //    painter->setPen(pen1);
    //    painter->drawEllipse(circleMiddle,outerCircle,outerCircle);
    //    if(innerCircle!=0)
    //    {
    //        painter->drawEllipse(circleMiddle,innerCircle,innerCircle);
    //    }
    //    if(innerCircle==0)
    //    {
    //        painter->drawPoint(circleMiddle);
    //    }
    //    painter->drawLine(int(radius+offset), int(radius+offset),
    //                     int(radius+offset-radius*cos(PAI/2+kUnit*angleLine1)), int(radius+offset-radius*sin(PAI/2+kUnit*angleLine1)));
    //    painter->drawLine(int(radius+offset), int(radius+offset),
    //                     int(radius+offset-radius*cos(PAI/2+kUnit*angleLine2)), int(radius+offset-radius*sin(PAI/2+kUnit*angleLine2)));


    // 显示所在处的两段线段
    //    painter->drawLine(int(radius+offset-innerCircle*cos(PAI/2+kUnit*angleLine1)), int(radius+offset-innerCircle*sin(PAI/2+kUnit*angleLine1)),
    //                      int(radius+offset-outerCircle*cos(PAI/2+kUnit*angleLine1)), int(radius+offset-outerCircle*sin(PAI/2+kUnit*angleLine1)));
    //    painter->drawLine(int(radius+offset-innerCircle*cos(PAI/2+kUnit*angleLine2)), int(radius+offset-innerCircle*sin(PAI/2+kUnit*angleLine2)),
    //                      int(radius+offset-outerCircle*cos(PAI/2+kUnit*angleLine2)), int(radius+offset-outerCircle*sin(PAI/2+kUnit*angleLine2)));

    /// 显示所在处的两段弧
    // drawArc无法完成任务
    //void QPainter::drawArc( int x, int y, int width, int height, int startAngle, int spanAngle )
    //outerCircle, angleLine1
    //    painter->drawArc( int(offset+rUnit*outerCircleID), int(offset+rUnit*outerCircleID),
    //                      int((radius -rUnit*outerCircleID)*2), int((radius -rUnit*outerCircleID)*2),
    //                      int(angleLine1ID*turnAngleDegree+90), int(turnAngleDegree) );   /// still not right
    //innerCircle, angleLine2
    //    painter->drawArc( int(offset+rUnit*innerCircleID), int(offset+rUnit*innerCircleID),
    //                      int((radius -rUnit*innerCircleID)*2), int((radius -rUnit*innerCircleID)*2),
    //                      int(angleLine2ID*turnAngleDegree+90), int(turnAngleDegree));

    /// 将所在空间单独显示出来
    QPen pen2;
    pen2.setColor(Qt::white);
    pen2.setWidth(5);
    painter->setPen(pen2);

    // innerCircle==-2  鼠标不在范围内
    // innerCircle==-1   鼠标在中心区域
    // innerCircle>=0    鼠标在常规网格区域
    /// 这是在有四个点的情况下
    if(innerCircle!=-1 && innerCircle!=-2)
    {
        QPainterPath path1;
        path1.moveTo(int(radius+offset-innerCircle*cos(PAI/2+kUnit*angleLine1)), int(radius+offset-innerCircle*sin(PAI/2+kUnit*angleLine1)));
        path1.lineTo(int(radius+offset-outerCircle*cos(PAI/2+kUnit*angleLine1)), int(radius+offset-outerCircle*sin(PAI/2+kUnit*angleLine1)));
        /// arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
        path1.arcTo(int(offset+rUnit*outerCircleID), int(offset+rUnit*outerCircleID),
                    int((radius -rUnit*outerCircleID)*2), int((radius -rUnit*outerCircleID)*2),
                    int(angleLine1ID*turnAngleDegree+90), int(turnAngleDegree));
        painter->drawPath(path1);

        QPainterPath path2;
        path2.moveTo(int(radius+offset-innerCircle*cos(PAI/2+kUnit*angleLine1)), int(radius+offset-innerCircle*sin(PAI/2+kUnit*angleLine1)));
        /// arcTo ( qreal x, qreal y, qreal width, qreal height, qreal startAngle, qreal sweepLength )
        path2.arcTo(int(offset+rUnit*innerCircleID), int(offset+rUnit*innerCircleID),
                    int((radius -rUnit*innerCircleID)*2), int((radius -rUnit*innerCircleID)*2),
                    int(angleLine1ID*turnAngleDegree+90), int(turnAngleDegree));
        painter->drawPath(path2);

        painter->drawLine(int(radius+offset-innerCircle*cos(PAI/2+kUnit*angleLine2)), int(radius+offset-innerCircle*sin(PAI/2+kUnit*angleLine2)),
                          int(radius+offset-outerCircle*cos(PAI/2+kUnit*angleLine2)), int(radius+offset-outerCircle*sin(PAI/2+kUnit*angleLine2)));
    }
    /// 3个点的情况下
    if(innerCircle==-1)
    {
        QPainterPath path3;
        path3.moveTo(circleMiddle);
        // 需要最小半径,outerCircle已经记录
        // 需要起始角度
        double x=minRadius*cos(PAI/2+kUnit*angleLine1ID);
        double y=minRadius*sin(PAI/2+kUnit*angleLine1ID);
        x=-x;
        y=-y;
        path3.lineTo(radius+offset+x,radius+offset+y);     //估计是这个出问题,的确，一直不变换
        //        qDebug()<<radius+offset+x<<radius+offset+y;
        path3.arcTo(offset+rUnit*minRadiusID, offset+rUnit*minRadiusID,
                    minRadius*2, minRadius*2,
                    angleLine1ID*turnAngleDegree+90, turnAngleDegree);  /// 此项注意要改长度和角度
        path3.lineTo(circleMiddle.x(),circleMiddle.y());

        painter->drawPath(path3);
    }
}
