#ifndef COLORTABLE_H
#define COLORTABLE_H

#include <QWidget>

class colorTable : public QWidget
{
    Q_OBJECT
public:
    explicit colorTable(QWidget *parent = 0);

private:
    QVector<QRgb> cTable;
    int minNum;
    int maxNum;
    int length;
    int offset;
    float diameter;
    int circleWidth;
    int circleHeight;

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void setColorTable();
signals:

public slots:
    void setRange(int min, int max);
    void setLength(int len,int off);
    void resizeWithCircle(int wid, int hei);

};

#endif // COLORTABLE_H
