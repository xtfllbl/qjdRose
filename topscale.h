#ifndef TOPSCALE_H
#define TOPSCALE_H

#include <QWidget>

class topScale : public QWidget
{
    Q_OBJECT
public:
    explicit topScale(QWidget *parent = 0);

    int radius;
    int offset;
    int mouseX;
    float diameter;
    int minOffset;
    int maxOffset;
private:
    void paintEvent(QPaintEvent *);
    void paintPosLine(QPainter *painter);

signals:

public slots:
    void setLength(int rad,int off);
    void setPosLine(int x);
    void setOffset(float min,float max);
    void resizeWithCircle(int wid,int hei);
};

#endif // TOPSCALE_H
