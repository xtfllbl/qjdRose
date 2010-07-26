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
    void setOffsetUnit(int);
    void setAzimuthUnit(int);
private:
    QJDObservationAttribute oa;
    int radius;
    double kUnit;
    qreal rUnit;
    qreal turnAngleDegree;

    QVector<QRgb> colorTable;
    QVector<QVector<int> > originUnitData;
    QVector<QVector<int> > colorUnitData;
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

    int maxOriginUnit;
    int minOriginUnit;
    int cutNum;
    int length;

    int circleNumber;
    int angleLineNumber;
    int offset;

    int mouseX;
    int mouseY;
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
    bool isStarted;

    void paintEvent(QPaintEvent *);
    void paintCurrentUnit(QPainter *painter);
    void paintAngle(QPainter *painter);

    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void setColorTable();
    void setData();
    void setOaData();
    void setOffset(int angle);

signals:
    void sigGetRange(int min,int max);
    void sigGetLength(int rad,int offset);
    void sigSetOffset(float minOffset, float maxOffset);
    void sigCurrentMousePos(int x,int y);
    void sigCurrentMousePosX(int x);
    void sigCurrentMousePosY(int y);
    void sigCurrentFold(int data);
    void sigCurrentOffset(int data);
    void sigCurrentAzimuth(int data);
    void sigWidgetSize(int wid,int hei);
public slots:
    void start();
};

#endif // QJDROSE_H
