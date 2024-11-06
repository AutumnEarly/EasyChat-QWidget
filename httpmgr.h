#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "singleton.h"
#include "global.h"

/*********************************************************************************
  * @Version:
  * @FileName:      httpmgr.h
  * @Brief:         http连接管理

  * @Author:        秋初
  * @Date:          2024-09-07
  * @History:
**********************************************************************************/


class HttpMgr : public QObject, public Singleton<HttpMgr>,
                public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT
    friend class Singleton<HttpMgr>;
public:
    ~HttpMgr();
    void PostHttpReq(QUrl url, QJsonObject obj, ReqId reqId, Modules mod);
signals:
    void sig_httpFinish(ReqId reqId, QString res, ErrorCodes err, Modules mod);
    // 注册模块类型请求信号完成
    void sig_reg_mod_finish(ReqId reqId, QString res, ErrorCodes err);
    void sig_login_mod_finish(ReqId reqId, QString res, ErrorCodes err);
private slots:
    void slot_httpFinish(ReqId reqId, QString res, ErrorCodes err, Modules mod);

private:
    HttpMgr();

    QNetworkAccessManager m_manager;
};

#endif // HTTPMGR_H
