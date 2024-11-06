#ifndef CHATTINGRECORDSLIST_H
#define CHATTINGRECORDSLIST_H

#include <QWidget>
#include <QScrollArea>
#include <QLayout>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QEvent>
#include <QStyleOption>
#include <QPainter>
#include <QListWidgetItem>
#include <QQueue>
#include "global.h"
#include "chatmsgitembase.h"
#include "../usermgr.h"

class ChattingRecordsList : public QWidget
{
    Q_OBJECT
    template<typename T>
    using shared_ptr = std::shared_ptr<T>;

public:

    explicit ChattingRecordsList(QWidget *parent = nullptr);
    ~ChattingRecordsList();

    void setUserInfo(std::shared_ptr<UserInfo> userInfo);
    std::shared_ptr<UserInfo> getUserInfo();

    // 尾插 消息信息
    void appendChatMsg(std::shared_ptr<UserInfo> userInfo, std::shared_ptr<ChatMsgData> msgData, bool isAdd = true);
    // 头插 消息信息
    void prependChatMsg(std::shared_ptr<UserInfo> userInfo, std::shared_ptr<ChatMsgData> msgData, bool isAdd = true);

    // 清空聊天框
    void clearChatItem();

    // 滚动到底部
    void scrollToBottom();
private:

    // TODO: 2024-10-17 待做
    // 暂且在消息上方添加一个时间提示标签
    // 添加一个提示时间组件
    void addTipItem(int index, const QString & tip);

    // 尾插 消息组件
    void appendChatItem(shared_ptr<UserInfo> userInfo, shared_ptr<ChatMsgData> msgData);
    // 头插 消息组件
    void prependChatItem(shared_ptr<UserInfo> userInfo, shared_ptr<ChatMsgData> msgData);
    // 指定插入 消息组件
    void insertChatItem(QWidget* item, QWidget* beforeItem);

public slots:
    void slot_loadItem(int direction);

    void slot_loadItem_accomplish();

    void slot_appendedFriendMsg(int uid, QVector<std::shared_ptr<ChatMsgData>> msgData);
signals:
    void sig_loadItem(int direction);
    void sig_loadItem_accomplish();

private slots:
    void onVScroolBatMoved(int min, int max);    
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    void initStyleSheet();
    bool m_isAppended;
    int m_maxLoadCount;
    int m_count;

    QScrollArea* m_scrollArea;
    QScrollBar* m_vScroolBar;
    QWidget* m_view_background;
    QWidget* m_view_content;

    QMap<int, QPixmap> m_usersHeadImg;
    std::shared_ptr<UserInfo> m_userInfo;
    QVector<QPair<shared_ptr<UserInfo>,shared_ptr<ChatMsgData>>> m_data;
    QQueue<QPair<shared_ptr<UserInfo>,shared_ptr<ChatMsgData>>> m_headCache;
    QQueue<QPair<shared_ptr<UserInfo>,shared_ptr<ChatMsgData>>> m_tailCache;

};

#endif // CHATTINGRECORDSLIST_H
