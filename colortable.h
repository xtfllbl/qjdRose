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

    void paintEvent(QPaintEvent *);
    void setColorTable();
signals:

public slots:
    void setRange(int min, int max);
    void setLength(int len,int off);

};

#endif // COLORTABLE_H
