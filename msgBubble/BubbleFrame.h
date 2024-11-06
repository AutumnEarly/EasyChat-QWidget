#ifndef BUBBLE_H
#define BUBBLE_H

#include <QFrame>
#include "global.h"
#include <QHBoxLayout>
#include <QColor>

/**
 * @brief The BubbleFrame class 聊天消息体的背景，是一个气泡形状的
 */
class BubbleFrame : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QColor backColor READ backColor WRITE setBackColor NOTIFY backColorChanged FINAL)
    Q_PROPERTY(ChatMsgRole role READ role WRITE setRole NOTIFY roleChanged FINAL)

public:
    BubbleFrame(ChatMsgRole role, QWidget *parent = nullptr);
    void setMargin(int margin);

    void setWidget(QWidget *w);

    QColor backColor() const;
    void setBackColor(const QColor &newBackColor);

    ChatMsgRole role() const;
    void setRole(ChatMsgRole newRole);

signals:
    void backColorChanged();

    void roleChanged();

protected:
    void paintEvent(QPaintEvent *e);
private:
    QHBoxLayout *m_pHLayout;
    ChatMsgRole m_role;
    int      m_margin;
    QColor m_backColor;


};

#endif // BUBBLE_H
