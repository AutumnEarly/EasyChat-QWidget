
#include "PictureBubble.h"



PictureBubble::PictureBubble(ChatMsgRole role, const QPixmap &picture, QWidget *parent)
    :BubbleFrame(role, parent)
{
    this->setBackColor(QColor(255,255,255,0));
    this->setRole(role);

    QLabel *lb = new QLabel();
    lb->setScaledContents(true);
//    QPixmap pix = picture.scaled(QSize(PIC_MAX_WIDTH, PIC_MAX_HEIGHT), Qt::KeepAspectRatio);
    QPixmap pix = picture;
    lb->setPixmap(pix);
    this->setWidget(lb);

    int left_margin = this->layout()->contentsMargins().left();
    int right_margin = this->layout()->contentsMargins().right();
    int v_margin = this->layout()->contentsMargins().bottom();
    setFixedSize(pix.width()+left_margin + right_margin, pix.height() + v_margin *2);

    if(this->role() == ChatMsgRole::Self) {
        this->setBackColor(QColor("#9EEA6A"));
    } else {
        this->setBackColor(QColor("#FFFFFF"));
    }
}

PictureBubble::PictureBubble(ChatMsgRole role, const QString &imgUrl, QWidget *parent)
    :BubbleFrame(role, parent)
{
    this->setBackColor(QColor(255,255,255,0));
    this->setRole(role);

    QUrl url(imgUrl);
    ImageLoader *imageLoader;
    if(url.isLocalFile()) {
        imageLoader = new ImageLoader(url.toLocalFile());
    } else {
        imageLoader = new ImageLoader(imgUrl);
    }
    QThread *thread = new QThread;

    imageLoader->moveToThread(thread);

    timer.start();
    connect(thread, &QThread::started, imageLoader, &ImageLoader::process);

    connect(imageLoader, &ImageLoader::imageLoaded, this, &PictureBubble::onImageLoaded);

    connect(imageLoader, &ImageLoader::finished, thread, &QThread::quit);

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

