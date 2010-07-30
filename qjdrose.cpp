#include "qjdrose.h"
#include <QDebug>

#include "math.h"
#define PAI 3.1415926
//#define CIRCLE 15     //offset
//#define ANGLE 16   //azimuth

/// TODO:
/// 2.实验窗口缩小的方法。。。
/// 3.成像错误
/// 4.圆形角度轴需要重绘
QJDRose::QJDRose(QWidget *)
{
    //    setOffsetUnit(30);   //无法让外部来得及调用
    //    setAzimuthUnit(36);
    setPalette(Qt::white);  // 无用
    offset=0;
    innerCircle=-2;

    setColorTable();
    isStarted=false;
    setMinimumSize(400,400);
//    setMaximumSize(2000,2000);  //因为需要扩充，照顾layout
//    this->sizePolicy().setHeightForWidth(true);  //无用？ ,仅仅设置prefer值
}

void QJDRose::start()
{
    setOaData();
    setData();
    isStarted=true;
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
    in.setVersion(QDataStream::Qt_4_5);  //! 异常重要，必须设置
    in.setByteOrder(QDataStream::LittleEndian);
    in.skipRawData(200);

    minOffset=100000;
    minAzimuth=100000;
    maxOffset=0;
    maxAzimuth=0;
    offsetData.clear();
    azimuthData.clear();
    /// 录入数据，兼顾求最大最小
    for(int i=0;i<oaNum;i++)
    {
        in>>oa;
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
        //        if(oa.azimuth<190 && oa.azimuth>180  && oa.offset>4500)  //检查数据与实际的差异
        //        {
        //            qDebug()<<oa.azimuth<<oa.offset;
        //        }
    }
    qDebug()<<"rose :: minOff:"<<minOffset<<" maxOff:"<<maxOffset;
    file.close();
}

void QJDRose::setData()
{
    originUnitData.clear();
    colorUnitData.clear();

    originUnitData.resize(circleNumber);
    for(int i=0;i<circleNumber;i++)
        originUnitData[i].resize(angleLineNumber);
    for(int i=0;i<circleNumber;i++)
        for(int j=0;j<angleLineNumber;j++)
            originUnitData[i][j]=0;

    colorUnitData.resize(circleNumber);
    for(int i=0;i<circleNumber;i++)
        colorUnitData[i].resize(angleLineNumber);
    for(int i=0;i<circleNumber;i++)
        for(int j=0;j<angleLineNumber;j++)
            colorUnitData[i][j]=0;
    /// -------------------归类-------------------- //
    // 网格划分,这一部分目前有隐藏的毛病
    float offsetUnitLength;
    float azimuthUnitLength;
    int gridX;
    int gridY;
    offsetUnitLength=(maxOffset-minOffset)/circleNumber;
    azimuthUnitLength=(maxAzimuth-minAzimuth)/angleLineNumber;
    for(int i=0;i<offsetData.size();i++)
    {
        // 减去最小值，除以网格长度，就能得到所处网格了
        gridX=int(ceil((offsetData[i]-minOffset)/offsetUnitLength)-1);
        gridY=int(ceil((azimuthData[i]-minAzimuth)/azimuthUnitLength)-1);  //此网格落的有问题
        // 不加限定网格会溢出
        if(gridX>circleNumber)
            gridX=circleNumber;
        if(gridX<0)
            gridX=0;
        if(gridY>angleLineNumber)
            gridY=angleLineNumber;
        if(gridY<0)
            gridY=0;

        originUnitData[circleNumber-gridX-1][gridY]++;  //为什么改的是x却会有改变，检查作图方法
    }

    /// --------------找到转换后数字的最大---------- ///
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

    cutNum=int(ceil(maxOriginUnit/255));
    for(int i=0;i<circleNumber;i++)
    {
        for(int j=0;j<angleLineNumber;j++)
        {
            if(originUnitData[i][j]!=0)
            {
                //不许大于255
                colorUnitData[i][j]=int(ceil(originUnitData[i][j]*1.0/(cutNum+1)));
            }
        }
        //        qDebug()<<"origin:: "<<i<<originUnitData[i];
        //        qDebug()<<"color:: "<<i<<colorUnitData[i];
        //        qDebug()<<"cutNum:: "<<cutNum<<"~~~~~~~~~";
    }
}

/// 需要调整一下画法，因为是从顶头开始并且顺时针，默认是逆时针，所以要取负值
void QJDRose::paintEvent(QPaintEvent *)
{
    if(isStarted==true)
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

        offset=length/8;
        radius=int((length/4)*1.5);
        kUnit=(2*PAI/angleLineNumber);
        rUnit=radius*1.0/circleNumber;
        int rUnitNum=int(radius/rUnit+1);

        emit sigGetLength(radius,offset);  /// 发送相关信息,还不如直接给半径来的直接
        emit sigSetOffset(minOffset,maxOffset);

        circleMiddle.setX(radius+offset);
        circleMiddle.setY(radius+offset);

        /// 填充网格
        /// 从0度顺时针绘图,success
        /// 在此，需要把所有的坐标保存下来，用于鼠标操作
        turnAngleDegree=-360*1.0/angleLineNumber;     //顺时针
        pointDataX.clear();
        pointDataY.clear();
        radiusData.clear();

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

        paintAngle(&painter);

        /// 发送相关信息，显示当前的数据情况
        int iii;
        int jjj;
        iii=outerCircleID;
        jjj=angleLine1ID;
        if(innerCircle!=-2)
        {
            emit sigCurrentFold(originUnitData[iii][jjj]);
        }
        if(innerCircle==-2)
        {
            emit sigCurrentFold(-2);
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
}

void QJDRose::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标移动事件，触发后还要重绘，影响了一定的系统效率

    if(isStarted==true)
    {
        mouseX=event->pos().x();
        mouseY=event->pos().y();

        /// ----------还能根据鼠标当前的位置确定鼠标处于哪圈与哪圈的中间，然后将圈圈画出----------///
        mouseRadius=sqrt((mouseX-circleMiddle.x())*(mouseX-circleMiddle.x())
                         +(mouseY-circleMiddle.y())*(mouseY-circleMiddle.y()));
        if(mouseRadius<=radius)
        {
            emit sigCurrentMousePosX(event->pos().x());
            emit sigCurrentMousePosY(event->pos().y());
        }
        else
        {
            emit sigCurrentMousePosX(-2);
            emit sigCurrentMousePosY(-2);
        }
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
        // 象限判断
        int angle;  //角度计算
        angle=int( acos ( (mouseX-circleMiddle.x())/mouseRadius) * 180.0 / PAI );
        if(mouseX>circleMiddle.x() && mouseY<circleMiddle.y())
        {
            temp1=0;
            temp2=temp;
            is1=true;
            angle=90-angle;
        }
        if(mouseX>circleMiddle.x() && mouseY>circleMiddle.y())
        {
            temp1=temp;
            temp2=temp*2;
            is2=true;
            angle=90+angle;
        }
        if(mouseX<circleMiddle.x() && mouseY>circleMiddle.y())
        {
            temp1=temp*2;
            temp2=temp*3;
            is3=true;
            angle=90+angle;
        }
        if(mouseX<circleMiddle.x() && mouseY<circleMiddle.y())
        {
            temp1=temp*3;
            temp2=temp*4;
            is4=true;
            angle=450-angle;
        }
        float mouseK;
        mouseK=(mouseY-circleMiddle.y())/(mouseX-circleMiddle.x());
        if(mouseRadius<=radius)
        {
            emit sigCurrentAzimuth(angle);  //发送当前角度数据
            /// 在这里直接计算offset算了，没必要链接过去
            setOffset(angle);
        }
        else
        {
            emit sigCurrentAzimuth(-2);  //发送无效信息
            emit sigCurrentOffset(-8000);  //-2在偏移范围内，找个范围外的
        }
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
}

/// 高亮当前鼠标滑过的模块
void QJDRose::paintCurrentUnit(QPainter *painter)
{
    /// 将所在空间单独显示出来
    QPen pen2;
    pen2.setColor(qRgb(255,255,150));   //黄色
    pen2.setWidth(3);
    painter->setPen(pen2);

    // innerCircle==-2  鼠标不在范围内
    // innerCircle==-1   鼠标在中心区域
    // innerCircle>=0    鼠标在常规网格区域
    /// 这是在有四个点的情况下
    if(innerCircle==-2)
    {
        return;
    }
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

/// 写上圆圈外的角度值
void QJDRose::paintAngle(QPainter *painter)
{
    // 整体定位
    //    painter->drawEllipse(circleMiddle,radius+30,radius+30);  //用来定位的圆圈
    QString angleText;
    QPointF writePos;
    for(int i=0;i<angleLineNumber;i++)
    {
        double x=(radius+15)*cos(PAI/2+kUnit*i);
        double y=(radius+15)*sin(PAI/2+kUnit*i);
        x=-x;
        y=-y;

        float angleNum=360*1.0/angleLineNumber*i;
        int showNum=int(angleNum);
        angleText=QString::number(showNum);
        writePos.setX(radius+offset+x);
        writePos.setY(radius+offset+y);
        if(i==0)
        {
            painter->drawText(int(writePos.x())-3, int(writePos.y())+5, "0");
        }
        if(i!=0)
        {
            painter->drawText(int(writePos.x()-12), int(writePos.y()+5), angleText);
        }
    }
}

void QJDRose::setOffsetUnit(int unit)
{
    circleNumber=unit;
}

void QJDRose::setAzimuthUnit(int unit)
{
    angleLineNumber=unit;
}

void QJDRose::setOffset(int angle)
{
    int tempOffset;
    int diameter=radius*2;
    if((angle>=0 && angle<=45)
        || (angle>=135 && angle<=225)
        || (angle>=315 && angle<=360))
        {
        if(mouseY>=offset && mouseY<=radius+offset)
        {
            tempOffset=int(-(maxOffset-minOffset)*(radius-(mouseY-offset))/radius)-minOffset;
        }
        if(mouseY>=radius+offset && mouseY<=diameter+offset)
        {
            tempOffset=int((maxOffset-minOffset)*(mouseY-radius-offset)/radius)+minOffset;
        }
    }
    if((angle>45 && angle<135)
        || (angle>225 && angle<315))
        {
        if(mouseX>=offset && mouseX<=radius+offset)
        {
            tempOffset=int(-(maxOffset-minOffset)*(radius-(mouseX-offset))/radius)-minOffset;
        }
        if(mouseX>=radius+offset && mouseX<=diameter+offset)
        {
            tempOffset=int((maxOffset-minOffset)*(mouseX-radius-offset)/radius)+minOffset;
        }
    }

    emit sigCurrentOffset(tempOffset);
}
