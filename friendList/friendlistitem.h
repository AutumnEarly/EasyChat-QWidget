#ifndef FRIENDLISTITEM_H
#define FRIENDLISTITEM_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

class FriendListItem : public QWidget
{
    Q_OBJECT
public:
    FriendListItem(QWidget * parent = nullptr);

    void setInfo(const QString& icon, const QString& text);

private:
    QLabel* m_icon;
    QLabel* m_text;
};

#endif // FRIENDLISTITEM_H
