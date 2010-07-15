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

private:
    QVector<QRgb> colorTable;
    QVector<qint64> originData;
    QVector<int> convetData;
    QVector<QVector<int> > colorData;
    int maxNum;
    int minNum;
    int cutNum;
    int length;

    int circleNumber;
    int angleLineNumber;
    int offset;


    void paintEvent(QPaintEvent *);
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
