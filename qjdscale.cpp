#include "qjdscale.h"
#include <QPainter>
QJDScale::QJDScale(QWidget *parent) :
    QWidget(parent)
{
    setMaximumWidth(90);
    setMinimumWidth(90);
}

void QJDScale::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawRect(1,1,width()-10,height()-10);
}
