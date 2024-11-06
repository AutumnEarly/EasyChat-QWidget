#ifndef CHATMSGITEMBASE_H
#define CHATMSGITEMBASE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include "global.h"


class ChatMsgItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChatMsgItemBase(QWidget* parent = nullptr,ChatMsgRole role = ChatMsgRole::Self);
    void setUserName(const QString& name);
    void setUserIcon(const QPixmap& icon, bool isScaled = false);
    void setWidget(QWidget* widget);
private:
    QVector<std::shared_ptr<ChatMsgData>> m_msgDatas;
    ChatMsgRole m_role;
    QLabel* m_nameLabel;
    QLabel* m_iconLabel;
    QWidget* m_content;

    QHBoxLayout* m_contentLayout;
};

#endif // CHATMSGITEMBASE_H
