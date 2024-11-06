#ifndef FRIENDAPPLYLIST_H
#define FRIENDAPPLYLIST_H

#include <QObject>
#include <QWidget>
#include <iostream>
#include <QListWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QScrollBar>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QImageReader>
#include <QThread>
#include <QMouseEvent>
#include "../components/buttonlabel.h"
#include "../usermgr.h"
#include "../tcpmgr.h"

class LoaderImage : public QObject
{
    Q_OBJECT

public:
    LoaderImage(const QString &imgUrl, int m_width, int m_height);


public slots:
    void process();
signals:
    void imageLoaded(const QPixmap& img);
    void finished();

private:
    QString m_imgUrl;
    int m_status;
    int m_width;
    int m_height;
};


class FriendApplyList : public QListWidget {
    Q_OBJECT
public:
    FriendApplyList(QWidget* parent = nullptr);
    ~FriendApplyList();

    void appendData(std::shared_ptr<FriendApplyInfo> info);
    void insertData(int index, std::shared_ptr<FriendApplyInfo> info);
    void removeData(int index);

    void loadCache();
public slots:
//    void onloadApplyListItem();
private:
    QVector<std::shared_ptr<FriendApplyInfo>> m_applyinfos;
    QVector<std::shared_ptr<FriendApplyInfo>> m_cache;
};
class ApplyListItem : public QWidget {
    Q_OBJECT
public:
    ApplyListItem(QWidget* parent = nullptr);
    ApplyListItem(const QString &name, const QString &backMsg,const QString& icon, QWidget* parent = nullptr);
    ApplyListItem(std::shared_ptr<FriendApplyInfo> info, QWidget*parent = nullptr);
    ~ApplyListItem();
private slots:
    void onImageLoaded(const QPixmap& img);
    void onAddButtonClicked(QMouseEvent* e);
public slots:

private:
    QLabel* m_icon;
    QLabel* m_name;
    QLabel* m_backMsg;
    ButtonLabel* m_addButton;
    std::shared_ptr<FriendApplyInfo> m_applyInfo;
};

#endif // FRIENDAPPLYLIST_H
