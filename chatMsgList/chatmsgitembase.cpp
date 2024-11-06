#include "chatmsgitembase.h"


#define ICON_MAX_H 50
#define ICON_MAX_W 50

ChatMsgItemBase::ChatMsgItemBase(QWidget* parent,ChatMsgRole role) :
    QWidget(parent), m_role(role)
{

    QHBoxLayout *pMainLayout = new QHBoxLayout();
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);


    // 创建用户名
    m_nameLabel = new QLabel();
    m_nameLabel->setObjectName("chat_msg_user_name");
    m_nameLabel->setFixedHeight(20);

    // 创建头像
    m_iconLabel = new QLabel();
    m_iconLabel->setObjectName("chat_msg_user_icon");
    m_iconLabel->setFixedSize(ICON_MAX_W,ICON_MAX_H);

    // 创建消息体
    m_content = new QWidget();
    m_content->setLayout(new QHBoxLayout());
    m_content->setObjectName("content_item");

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setObjectName("vl_headItem");
    vLayout->setMargin(0);

    QGridLayout *pGLayout = new QGridLayout();
    pGLayout->setObjectName("main_gLayout");
    pGLayout->setVerticalSpacing(3);
    pGLayout->setHorizontalSpacing(3);
    pGLayout->setMargin(3);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setSpacing(10);
    if(m_role == ChatMsgRole::Self)
    {
        QVBoxLayout* v1 = new QVBoxLayout();
        v1->setSpacing(10);
        QSpacerItem* p2 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        QHBoxLayout* l1 = new QHBoxLayout();
        l1->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
        l1->addWidget(m_nameLabel);

        QHBoxLayout* l3 = new QHBoxLayout();
        l3->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
        l3->addWidget(m_content);

        QVBoxLayout* l2 = new QVBoxLayout();
        l2->addWidget(m_iconLabel);
        l2->addItem(p2);

        v1->addLayout(l1);
        v1->addLayout(l3);

        mainLayout->addLayout(v1);
        mainLayout->addLayout(l2);

        m_contentLayout = l3;
    } else {
        QVBoxLayout* v1 = new QVBoxLayout();
        v1->setSpacing(10);
        QSpacerItem* p2 = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        QHBoxLayout* l1 = new QHBoxLayout();
        l1->addWidget(m_nameLabel);
        l1->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));

        QHBoxLayout* l3 = new QHBoxLayout();
        l3->addWidget(m_content);
        l3->addItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));


        QVBoxLayout* l2 = new QVBoxLayout();
        l2->addWidget(m_iconLabel);
        l2->addItem(p2);

        v1->addLayout(l1);
        v1->addLayout(l3);

        mainLayout->addLayout(l2);
        mainLayout->addLayout(v1);




        m_contentLayout = l3;
    }

//    QSpacerItem*pSpacer = new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    if(m_role == ChatMsgRole::Self)
//    {
//        m_nameLabel->setContentsMargins(0,0,8,0);
//        m_nameLabel->setAlignment(Qt::AlignRight);

//        pGLayout->addWidget(m_nameLabel, 0,1, 1,1);
//        pGLayout->addWidget(m_iconLabel, 0, 2, 2,1, Qt::AlignTop);
//        pGLayout->addItem(pSpacer, 1, 0, 1, 1, Qt::AlignLeft);

//        pGLayout->addWidget(m_content, 1, 1, 1, 1, Qt::Alignment(Qt::AnchorRight));

//        pGLayout->setColumnStretch(0, 2);
//        pGLayout->setColumnStretch(1, 3);

//    } else {
//        m_nameLabel->setContentsMargins(8,0,0,0);
//        m_nameLabel->setAlignment(Qt::AlignLeft);

//        pGLayout->addWidget(m_iconLabel, 0, 0, 2, 1, Qt::AlignTop);
//        pGLayout->addWidget(m_nameLabel, 0, 1, 1, 1);
//        pGLayout->addWidget(m_content, 1, 1, 1, 1);
//        pGLayout->addItem(pSpacer, 2, 2, 1, 1);
//        pGLayout->setColumnStretch(1, 3);
//        pGLayout->setColumnStretch(2, 2);
//    }

    pMainLayout->addLayout(mainLayout);
    this->setLayout(pMainLayout);
    this->setObjectName("ChatMsgItemBase");
}

void ChatMsgItemBase::setUserName(const QString &name)
{
    m_nameLabel->setText(name);
}

void ChatMsgItemBase::setUserIcon(const QPixmap &icon, bool isScaled)
{
    if(isScaled) {
        QPixmap img = icon.scaledToHeight(ICON_MAX_H,Qt::SmoothTransformation);
        m_iconLabel->setPixmap(img);
        return;
    }
    m_iconLabel->setPixmap(icon);
}

void ChatMsgItemBase::setWidget(QWidget *widget)
{
//    QGridLayout *pGLayout = this->findChild<QGridLayout*>("main_gLayout");
//    pGLayout->replaceWidget(m_content,widget);
//    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_contentLayout->replaceWidget(m_content, widget);

    delete m_content;
    m_content = widget;
    m_content->setObjectName("content_item");


}
