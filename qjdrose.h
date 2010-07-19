#ifndef QJDROSE_H
#define QJDROSE_H
#include <QtGui>
#include "colortable.h"
class QJDRose:public QWidget
{
    Q_OBJECT
public:
    QJDRose(QWidget *parent = 0);
    void emitRange();  //手动在connect完毕后发送信号
    void getCurrentPosData();

private:
    int radius;
    double kUnit;
    QVector<QRgb> colorTable;
    QVector<qint64> originData;
    QVector<int> convetData;
    QVector<QVector<int> > colorData;
    // 记录所有点的位置,一个空间貌似还不够
    QVector<QVector<int> > pointDataX;
    QVector<QVector<int> > pointDataY;
    QVector<int> radiusData;
    int minRediusData;
    QVector<float> angleLineK;  //这个不能为0，否则好多都为0了
    int maxNum;
    int minNum;
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
    int innerCircle;

    int angleLine1;
    int angleLine2;

    void paintEvent(QPaintEvent *);
    void paintCurrentUnit(QPainter *painter);

    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void setColorTable();
    void setData();
signals:
    void sigGetRange(int min,int max);
    void sigGetLength(int len,int offset);
    void sigCurrentMousePos(int x,int y);
    void sigCurrentMousePosX(int x);
    void sigCurrentMousePosY(int y);
};

#endif // QJDROSE_H
