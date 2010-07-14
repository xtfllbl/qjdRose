#ifndef COLORTABLE_H
#define COLORTABLE_H

#include <QWidget>

class colorTable : public QWidget
{
    Q_OBJECT
public:
    explicit colorTable(QWidget *parent = 0);
    void setRange(int min, int max);

private:
    QVector<QRgb> cTable;
    int minNum;
    int maxNum;

    void paintEvent(QPaintEvent *);
    void setColorTable();
signals:

public slots:

};

#endif // COLORTABLE_H
