#ifndef QJDROSE_H
#define QJDROSE_H
#include <QtGui>
#include "colortable.h"
#include "qjdobservationattribute.h"

class QJDRose:public QWidget
{
    Q_OBJECT
public:
    QJDRose(QWidget *parent = 0);
    void emitRange();  //手动在connect完毕后发送信号
    void getCurrentPosData();

private:
    QJDObservationAttribute oa;
    int radius;
    double kUnit;
    qreal rUnit;
    qreal turnAngleDegree;

    QVector<QRgb> colorTable;
    QVector<qint64> originData;
    QVector<int> convetData;
    QVector<QVector<int> > originUnitData;
    QVector<QVector<int> > colorUnitData;
    QVector<QVector<qint64> > originDataDouble;
    QVector<QVector<int> > colorData;
    // 记录所有点的位置,一个空间貌似还不够
    QVector<QVector<int> > pointDataX;
    QVector<QVector<int> > pointDataY;
    QVector<int> radiusData;

    // 数据记录
    QVector<float> offsetData;
    QVector<float> azimuthData;
    float minOffset;
    float maxOffset;
    float minAzimuth;
    float maxAzimuth;


    int minRadius;
    int minRadiusID;
    int maxRadius;
    int maxRadiusID;
    QVector<float> angleLineK;  //这个不能为0，否则好多都为0了
    int maxNum;
    int minNum;
    int maxOriginUnit;
    int minOriginUnit;
    int cutNum;
    int length;

    int circleNumber;
    int angleLineNumber;
    int offset;

    int mouseX;
    int mouseY;
    int missX;
    int missY;
//    QVector<int> nearX;
//    QVector<int> nearY;
    QPointF circleMiddle;  //圆心

    double mouseRadius;
    int outerCircle;
    int outerCircleID;
    int innerCircle;
    int innerCircleID;

    int angleLine1;
    int angleLine1ID;
    int angleLine2;
    int angleLine2ID;

    void paintEvent(QPaintEvent *);
    void paintCurrentUnit(QPainter *painter);

    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void setColorTable();
    void setData();
    void setOaData();
    void setData2();
signals:
    void sigGetRange(int min,int max);
    void sigGetLength(int len,int offset);
    void sigSetOffset(int minOffset,int maxOffset);
    void sigCurrentMousePos(int x,int y);
    void sigCurrentMousePosX(int x);
    void sigCurrentMousePosY(int y);
    void sigCurrentData(int data);
    void sigWidgetSize(int wid,int hei);
};

#endif // QJDROSE_H
