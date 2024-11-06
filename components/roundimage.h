#ifndef ROUNDIMAGE_H
#define ROUNDIMAGE_H

#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

class RoundImage : public QPixmap
{

public:
    RoundImage();
    RoundImage(const QPixmap& pixmap);
private:
    int m_radius;
};

#endif // ROUNDIMAGE_H
