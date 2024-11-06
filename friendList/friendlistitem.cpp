#include "friendlistitem.h"

#include "qctool.h"

FriendListItem::FriendListItem(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hLayout = new QHBoxLayout();

    m_icon = new QLabel();
    m_text = new QLabel();


    m_icon->setFixedSize(40,40);
    m_icon->setScaledContents(true);

    m_text->setFixedHeight(40);
    hLayout->addWidget(m_icon,Qt::AlignLeft);
    hLayout->addWidget(m_text);

    this->setLayout(hLayout);
}

void FriendListItem::setInfo(const QString &icon, const QString &text)
{
    m_text->setText(text);

    QPixmap img(icon);
    img = img.scaled(40,40,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    QPixmap pixmap(QCTool::RoundImage(img,40,40,5));

    m_icon->setPixmap(pixmap);
}
