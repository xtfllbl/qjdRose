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

    int length;
    int offset;
    int mouseY;
    float diameter;

    int minOffset;
    int maxOffset;
private:
    void paintEvent(QPaintEvent *);
    void paintPosLine(QPainter *painter);
signals:

public slots:
    void setLength(int len,int off);
    void setPosLine(int y);
    void setOffset(int min,int max);
};

#endif // QJDSCALE_H
