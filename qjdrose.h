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

    void paintEvent(QPaintEvent *);
    void setColorTable();
    void setData();


    int circleNumber;
    int angleLineNumber;
    int offset;
};

#endif // QJDROSE_H
