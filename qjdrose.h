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

    void paintEvent(QPaintEvent *);
    void setColorTable();
    void setData();


    int circleNumber;
    int angleLineNumber;
    int offset;
signals:
    void sigGetRange(int min,int max);
};

#endif // QJDROSE_H
