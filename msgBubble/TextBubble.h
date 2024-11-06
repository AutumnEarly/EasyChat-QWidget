#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

#include <QTextEdit>
#include "bubbleFrame.h"
#include <QHBoxLayout>

/**
 * @brief The TextBubble class 文本聊天气泡框
 */
class TextBubble : public BubbleFrame
{
    Q_OBJECT
public:
    TextBubble(ChatMsgRole role, const QString &text, QWidget *parent = nullptr);
protected:
    bool eventFilter(QObject *o, QEvent *e);
private:
    void adjustTextHeight();
    void setPlainText(const QString &text);
    void initStyleSheet();
private:
    QTextEdit *m_pTextEdit;
};

#endif // TEXTBUBBLE_H
