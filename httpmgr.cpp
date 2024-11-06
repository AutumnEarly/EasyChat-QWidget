#include "httpmgr.h"


HttpMgr::HttpMgr()
{
    connect(this,&HttpMgr::sig_httpFinish,this,&HttpMgr::slot_httpFinish);
}

HttpMgr::~HttpMgr()
{
    qDebug() << "HttpMgr delete~";
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject obj, ReqId reqId, Modules mod)
{
    //1. 准备发送请求的数据
    QByteArray data = QJsonDocument(obj).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.size()));
    auto self = shared_from_this();
    // 2. 发送post请求
    QNetworkReply *reply = m_manager.post(request,data);
    // 3. 绑定发送完成的信号
    connect(reply,&QNetworkReply::finished,[self,obj,reply,reqId,mod]() {
        // 发送失败
        if(reply->error() != QNetworkReply::NoError) {
            qDebug() << "http post error code is " << ErrorCodes::ERR_NETWORK
                     << "Request reqId: " << reqId << " mod: " << mod << " data: " << obj;
            emit self->sig_httpFinish(reqId,"",ErrorCodes::ERR_NETWORK,mod);
            reply->deleteLater();
            return;
        }
        // 读取回包
        QString res = reply->readAll();
        emit self->sig_httpFinish(reqId,res,ErrorCodes::SUCCESS,mod);
        reply->deleteLater();
    });
}
/**
 * @brief HttpMgr::slot_httpFinish post HTTP请求发送完成槽函数然后
 * 通过mod模块分发不同的模块完成信号
 * @param reqId 请求的id
 * @param res 返回的数据
 * @param err 错误码
 * @param mod 模块类型
 */
void HttpMgr::slot_httpFinish(ReqId reqId, QString res, ErrorCodes err, Modules mod)
{
    //发送信号通知注册模块http响应结束
    if(mod == Modules::REGISTERMOD) {
        emit sig_reg_mod_finish(reqId,res,err);
    }
    //发送信号通知登录模块http响应结束
    if(mod == Modules::LOGINMOD) {
        emit sig_login_mod_finish(reqId,res,err);
    }
    //发送信号通知登录模块http响应结束
    if(mod == Modules::RESETMOD) {
        emit sig_login_mod_finish(reqId,res,err);
    }
}
