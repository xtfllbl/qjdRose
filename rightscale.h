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
private:
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void setLength(int len,int off);
};

#endif // QJDSCALE_H
