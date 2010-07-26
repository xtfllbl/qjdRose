#ifndef QJDSCALE_H
#define QJDSCALE_H

#include <QWidget>

// 刻度轴
// 好好规划下，如何做刻度轴
class rightScale : public QWidget
{
    Q_OBJECT
public:
    explicit rightScale(QWidget *parent = 0);

    int radius;
    int offset;
    int mouseY;
    float diameter;

    int minOffset;
    int maxOffset;
    int angle;
private:
    void paintEvent(QPaintEvent *);
    void paintPosLine(QPainter *painter);
signals:
    void sigCurrentOffset(int off);
public slots:
    void setLength(int rad,int off);
    void setPosLine(int y);
    void setOffset(float min, float max);
    void resizeWithCircle(int wid, int hei);
    void setAngle(int ang);
};

#endif // QJDSCALE_H
