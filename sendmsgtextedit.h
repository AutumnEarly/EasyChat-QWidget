#ifndef SENDMSGTEXTEDIT_H
#define SENDMSGTEXTEDIT_H

#include <QTextEdit>
#include <QObject>
#include <QWidget>
#include <QDropEvent>
#include <QDebug>
#include <QMimeData>
#include <QClipboard>
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QMouseEvent>
#include <QApplication>
#include <QTextDocument>
#include <QTextBlock>
#include "global.h"

class SendMsgTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    SendMsgTextEdit(QWidget* parent = Q_NULLPTR);
    void insertFileFromUrl(const QList<QUrl>& urls);
    QVector<MsgInfo> getMsgList();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    bool isImage(const QUrl& url);
    void insertImage(const QUrl& url);
    void insertImage(const QImage& url);
    void insertMsgList(QVector<MsgInfo> &list,QString flag, QString text, QPixmap pix);
    QList<QUrl> getUrl(QString urls);

    // 存储一些url信息，如图片等
    QVector<MsgInfo> m_urlList;
    QVector<MsgInfo> m_getMsgList;
};

#endif // SENDMSGTEXTEDIT_H
