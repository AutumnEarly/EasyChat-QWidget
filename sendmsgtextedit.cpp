#include "sendmsgtextedit.h"

SendMsgTextEdit::SendMsgTextEdit(QWidget* parent) : QTextEdit(parent)
{

}

void SendMsgTextEdit::insertFileFromUrl(const QList<QUrl>& urls)
{
    if(urls.isEmpty()) return;

    foreach (QUrl url, urls) {
        if(!url.isLocalFile()) return;

        if(isImage(url)) {
            insertImage(url);
        } else {

        }
    }
}

QVector<MsgInfo> SendMsgTextEdit::getMsgList()
{

    m_getMsgList.clear();
    QString doc = this->document()->toPlainText();
    QString text = ""; // 存储文本信息
    int indexUrl = 0; // url文件
    int count = m_urlList.size(); // url

    // 以字节循环
    for(int index=0; index<doc.size(); index++)
    {

        // 遇到特殊字符，就代表可能是之前插入的图片或文件
        if(doc[index]==QChar::ObjectReplacementCharacter)
        {
            qDebug() << "char " << doc[index];
            if(!text.isEmpty())
            {
                QPixmap pix;
                insertMsgList(m_getMsgList,"text",text,pix);
                text.clear();
            }
            // 遍历之前插入的文件，通过转成html然后搜寻文件的名字
            while(indexUrl<count)
            {

                MsgInfo msg = m_urlList[indexUrl];
                if(this->document()->toHtml().contains(msg.content,Qt::CaseSensitive))
                {
//                    qDebug() << "add msg " << msg.msgFlag << " content " << msg.content;
                    indexUrl++;
                    m_getMsgList.append(msg);
                    break;
                }
                indexUrl++;
            }
        }
        else
        {
            text.append(doc[index]);
        }
    }

    // 去除两端空白字符
    text = text.trimmed();
    if(!text.isEmpty())
    {
        QPixmap pix;
        insertMsgList(m_getMsgList,"text",text,pix);
        text.clear();
    }
    m_urlList.clear();
    this->clear();
    return m_getMsgList;
}

void SendMsgTextEdit::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "dragEnter ";
    if(event->source() == this) {
        event->ignore();
    } else{
        event->accept();
    }

}

void SendMsgTextEdit::dropEvent(QDropEvent *event)
{
    qDebug() << "dropEvent ";

    const QList<QUrl> urls = event->mimeData()->urls();

    // 处理url文件信息
    insertFileFromUrl(urls);
    event->accept();

}

void SendMsgTextEdit::keyPressEvent(QKeyEvent *event)
{
    bool isEvent = true;
    if (event->matches(QKeySequence::Paste)) {
        // 处理粘贴操作，获取剪贴板中的数据并插入到QTextEdit中
        QClipboard* clipboard = QGuiApplication::clipboard();
        const QMimeData* mimeData = clipboard->mimeData();

        if (mimeData->hasUrls()) {
            QList<QUrl> imageUrls = mimeData->urls();
            for(auto & imgUrl : imageUrls) {
                if(isImage(imgUrl)) {
                    this->insertImage(imgUrl);
                    isEvent = false;
                }
            }
        }
    }

    if(isEvent) {
        QTextEdit::keyPressEvent(event);
    }

}

bool SendMsgTextEdit::isImage(const QUrl &url)
{
    QString filePath = url.toLocalFile();
    QList<QString> suffixs  {"jpg","png","gif","webp"};
    QFileInfo fileInfo(filePath);
    if (suffixs.indexOf(fileInfo.suffix()) >= 0) {
        return true;
    }
    qDebug() << "url is no image url!";
    return false;
}

void SendMsgTextEdit::insertImage(const QUrl &url)
{
    if (url.isLocalFile()) {
        QImage image(url.toLocalFile());
        QImage scaledImage; // 缩放图片
        if (!image.isNull()) {
            // 计算缩放比例
            double scaleFactor = qMin((double)width() / image.width(), (double)height() / image.height());
            int newWidth = image.width() * scaleFactor;
            int newHeight = image.height() * scaleFactor;
            if(newWidth > newHeight) {
                scaledImage = image.scaledToWidth(newWidth,Qt::SmoothTransformation);
            } else {
                scaledImage = image.scaledToHeight(newHeight,Qt::SmoothTransformation);
            }

            QTextCursor tCursor = textCursor();
            tCursor.insertImage(scaledImage,url.toString());
//            tCursor.insertBlock();
            setTextCursor(tCursor);
            insertMsgList(m_urlList,"image",url.toString(),QPixmap::fromImage(scaledImage));
        }
    }

}

void SendMsgTextEdit::insertMsgList(QVector<MsgInfo> &list, QString flag, QString text, QPixmap pix)
{
    MsgInfo msg;
    msg.msgFlag = flag;
    msg.content = text;
    msg.pixmap = pix;
    list.append(msg);
}

QList<QUrl> SendMsgTextEdit::getUrl(QString text)
{
    QList<QUrl> urls;
    if(text.isEmpty()) return urls;

    QStringList list = text.split("\n");
    foreach (QString url, list) {
        if(!url.isEmpty()){
            urls.append(url);
        }
    }

    return urls;
}

