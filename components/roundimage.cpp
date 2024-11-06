#include "roundimage.h"

RoundImage::RoundImage() : m_radius(10)
{

}

RoundImage::RoundImage(const QPixmap& pixmap)
{
    // 创建一个透明的圆角矩形
    QPixmap roundedImage(pixmap.width(), pixmap.height());
    roundedImage.fill(Qt::transparent);
    QPainter painter(this);

    // 绘制圆角矩形
    QPainterPath path;
    path.addRoundedRect(0, 0, pixmap.width(), pixmap.height(), m_radius, m_radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, pixmap);
}



