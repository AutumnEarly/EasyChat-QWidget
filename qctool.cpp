#include "qctool.h"

QCTool::QCTool()
{

}

QPixmap QCTool::RoundImage(const QPixmap& pix,int w, int h, int radius)
{
    QPixmap roundedImage(pix);
    roundedImage.fill(Qt::transparent);

    QPainterPath path;
    path.addRoundedRect(0,0,w,h,radius,radius);

    QPainter painter(&roundedImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipPath(path,Qt::ReplaceClip);
    painter.drawPixmap(0, 0, pix); // 绘制原始图片

    return roundedImage;
}
