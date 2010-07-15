#ifndef TOPSCALE_H
#define TOPSCALE_H

#include <QWidget>

class topScale : public QWidget
{
    Q_OBJECT
public:
    explicit topScale(QWidget *parent = 0);

    int length;
    int offset;
private:
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void setLength(int len,int off);
};

#endif // TOPSCALE_H
