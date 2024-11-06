#ifndef QCTOOL_H
#define QCTOOL_H

#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

class QCTool
{
public:
    QCTool();
    static QPixmap RoundImage(const QPixmap& pix,int w, int h, int radius);

};

#endif // QCTOOL_H
