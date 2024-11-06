#ifndef PICTUREBUBBLE_H
#define PICTUREBUBBLE_H

#include "bubbleFrame.h"
#include <QHBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QImageReader>
#include <QThread>
#include <QElapsedTimer>
#include <QtConcurrent>

#define PIC_MAX_WIDTH 400
#define PIC_MAX_HEIGHT 200



class ImageLoader : public QObject
{
    Q_OBJECT

public:
    ImageLoader(const QString &imgUrl) : m_imgUrl(imgUrl),m_status(0) {}

public slots:
    void process() {
        QImageReader reader(m_imgUrl);
        reader.setAutoTransform(true);
        reader.setDecideFormatFromContent(true);

        QImage image = reader.read();
        image = image.scaledToHeight(PIC_MAX_HEIGHT,Qt::SmoothTransformation);

        if (image.isNull()) {
            emit imageLoaded(image);
        } else {
            emit imageLoaded(image);
        }

        emit finished();
    }

signals:
    void imageLoaded(const QImage& img);
    void finished();

private:
    QString m_imgUrl;
    int m_status;
};

/**
 * @brief The PictureBubble class 图片聊天气泡
*/

class PictureBubble : public BubbleFrame
{
    Q_OBJECT
public:
    PictureBubble(ChatMsgRole role, const QPixmap &picture, QWidget *parent = nullptr);
    PictureBubble(ChatMsgRole role, const QString &url, QWidget *parent = nullptr);
    // 在PictureBubble类中添加槽函数
private slots:
    void onImageLoaded(const QImage& img) {

        int left_margin = this->layout()->contentsMargins().left();
        int right_margin = this->layout()->contentsMargins().right();
        int v_margin = this->layout()->contentsMargins().bottom();
        this->setFixedSize(img.width() + left_margin + right_margin, img.height() + v_margin * 2);

        if(this->role() == ChatMsgRole::Self) {
            this->setBackColor(QColor("#9EEA6A"));
        } else {
            this->setBackColor(QColor("#FFFFFF"));
        }
        QLabel *lb = new QLabel();
        QPixmap pixmap = QPixmap::fromImage(img);
        lb->setPixmap(pixmap);
        this->setWidget(lb);

        qint64 elapsed = timer.elapsed(); // 获取经过的时间，单位为毫秒
        qDebug() << "create Img time: " << elapsed << "ms";
    }
public:
    QElapsedTimer timer;

signals:

private:


};

#endif // PICTUREBUBBLE_H
