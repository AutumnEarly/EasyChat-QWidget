#pragma execution_character_set("utf-8")

#include "friendapplylist.h"

FriendApplyList::FriendApplyList(QWidget *parent) : QListWidget(parent)
{
    auto data = UserMgr::GetInstance()->getApplyList();
    for(auto info : data) {
        this->appendData(info);
    }
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_notify_addFriendApply,this,[&](std::shared_ptr<FriendApplyInfo> info) {
        this->appendData(info);
    });
}

FriendApplyList::~FriendApplyList()
{

}

void FriendApplyList::appendData(std::shared_ptr<FriendApplyInfo> info)
{
    if(this->isHidden()) {
        m_cache.push_back(info);
    } else {
        m_applyinfos.push_back(info);
        ApplyListItem* item = new ApplyListItem(info);
        QListWidgetItem* listItem = new QListWidgetItem();
        listItem->setSizeHint(QSize(200,80));
        this->addItem(listItem);
        this->setItemWidget(listItem, item);
    }

}

void FriendApplyList::insertData(int index, std::shared_ptr<FriendApplyInfo> info)
{

}

void FriendApplyList::removeData(int index)
{
    QListWidgetItem* item = this->takeItem(index);
    m_applyinfos.remove(index);
    if(item) {
        delete item;
    }

}

void FriendApplyList::loadCache()
{
    qDebug() << "load cache ~";

    foreach (auto info, m_cache) {
        ApplyListItem* item = new ApplyListItem(info);
        QListWidgetItem* listItem = new QListWidgetItem();
        listItem->setSizeHint(QSize(200,80));
        this->addItem(listItem);
        this->setItemWidget(listItem, item);
    }

    m_cache.clear();
}

ApplyListItem::ApplyListItem(QWidget *parent) : QWidget(parent)
{

}

ApplyListItem::ApplyListItem(const QString &name, const QString &backMsg, const QString &icon, QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* vLayout = new QVBoxLayout();

    mainLayout->setMargin(20);

    m_name = new QLabel(name);
    m_name->setFixedHeight(25);

    m_backMsg = new QLabel(backMsg);
    m_backMsg->setFixedHeight(25);

    m_icon = new QLabel();
    m_icon->setFixedSize(QSize(50,50));
    m_icon->setAlignment(Qt::AlignCenter);
    m_icon->setScaledContents(true);
    LoaderImage *imageLoader = new LoaderImage(icon,40,40);
    QThread *thread = new QThread;
    imageLoader->moveToThread(thread);
    connect(thread, &QThread::started, imageLoader, &LoaderImage::process);
    connect(imageLoader, &LoaderImage::imageLoaded, this, &ApplyListItem::onImageLoaded);
    connect(imageLoader, &LoaderImage::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();

    m_addButton = new ButtonLabel();
    m_addButton->setText("添加");
    m_addButton->setFixedWidth(100);
    m_addButton->setAlignment(Qt::AlignCenter);
    m_addButton->setObjectName("add_friend_btn");
    connect(m_addButton,&ButtonLabel::clicked,this,&ApplyListItem::onAddButtonClicked);
    vLayout->addWidget(m_name);
    vLayout->addWidget(m_backMsg);


    mainLayout->addWidget(m_icon);
    mainLayout->addLayout(vLayout);
    mainLayout->addWidget(m_addButton);
    this->setLayout(mainLayout);


}

ApplyListItem::ApplyListItem(std::shared_ptr<FriendApplyInfo> info, QWidget *parent) :
    ApplyListItem(info->_name,info->_back_msg,info->_icon,parent)
{
    m_applyInfo = info;

    if(m_applyInfo->_status == 1) {
        m_addButton->setText(QString::fromLocal8Bit("已添加 "));
        m_addButton->setEnabled(false);
        m_addButton->setState("added");

    } else {
        m_addButton->setText("添加");
        m_addButton->setState("normal");
    }


}

ApplyListItem::~ApplyListItem()
{

}

void ApplyListItem::onImageLoaded(const QPixmap &img)
{
    m_icon->setPixmap(img);
}

void ApplyListItem::onAddButtonClicked(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton) {
        QJsonObject obj;
        obj["fromuid"] = UserMgr::GetInstance()->getUid();
        obj["touid"] = m_applyInfo->_uid;
        obj["nickname"] = "暂无备注";
        obj["tag"] = "标签";
        obj["device"] = this_device_name;

        QByteArray data = QJsonDocument(obj).toJson();
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_AUTH_FRIEND_REQ,data);
    }
}

LoaderImage::LoaderImage(const QString &imgUrl, int m_width, int m_height) :
    m_imgUrl(imgUrl), m_width(m_width), m_height(m_height)
{

}

void LoaderImage::process() {
    QUrl url(m_imgUrl);
    QImageReader reader;
    if(url.isLocalFile()) {
        reader.setFileName(url.toLocalFile());
    } else {
        reader.setFileName(m_imgUrl);
    }

    reader.setAutoTransform(true);
    reader.setDecideFormatFromContent(true);

    QImage image = reader.read();
    image = image.scaled(m_width,m_height,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);

    if (image.isNull()) {
        emit imageLoaded(QPixmap::fromImage(image));
    } else {
        emit imageLoaded(QPixmap::fromImage(image));
    }

    emit finished();
}
