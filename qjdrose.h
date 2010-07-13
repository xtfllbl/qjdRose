#ifndef QJDROSE_H
#define QJDROSE_H
#include <QtGui>

class QJDRose:public QWidget
{
    Q_OBJECT
public:
    QJDRose(QWidget *parent = 0);

private:
    QVector<QRgb> colorTable;
    QVector<qint64> originData;
    QVector<int> convetData;
    QVector<QVector<int> > colorData;

    void paintEvent(QPaintEvent *);
    void setColorTable();
    void setData();


    int circleNumber;
    int angleLineNumber;
    int offset;
};

#endif // QJDROSE_H
