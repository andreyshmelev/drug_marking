#include "apimark.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

APIMARK::APIMARK(QObject *parent) : QObject(parent)
{

    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyfinished(QNetworkReply*)));

    QByteArray data = ("{\"client_id\": \"ef77a1f8-e374-451d-9da9-7c3519d0d143\",\"client_secret\": \"c4bf1684-eb4e-4119-bed7-b28fc3beb68b\",\"user_id\": \"test_non_resident\",\"auth_type\": \"PASSWORD\"}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/auth"); // авторизируемся туточки

    requestauthorization.setUrl(serviceURL);
    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Cache-Control","no-cache");

    QNetworkReply *reply = manager->post(requestauthorization,data);
}


void APIMARK::replyfinished(QNetworkReply *reply)
{
    int i;
    QByteArray bytes = reply->readAll(); // bytes

    QString stringreply = QString::fromUtf8(bytes);
    qDebug() << "Server reply "<< stringreply;

    if (reply->errorString() != "Unknown error"){
        qDebug() << "API REQUEST ERROR "<< reply->errorString();
    }

    // read and parse reply


    QJsonDocument loadDoc(QJsonDocument::fromJson(bytes));
    QJsonObject jsonObject = loadDoc.object();
    QString code =  jsonObject["code"].toString();

    if (!code.isEmpty()){
        qDebug() << "code" << code;
    }
}


QString APIMARK::GetCodeAuth()
{
    QByteArray data = ("{\"client_id\": \"ef77a1f8-e374-451d-9da9-7c3519d0d143\",\"client_secret\": \"c4bf1684-eb4e-4119-bed7-b28fc3beb68b\",\"user_id\": \"test_non_resident\",\"auth_type\": \"PASSWORD\"}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/auth"); // авторизируемся туточки

    requestauthorization.setUrl(serviceURL);
    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Cache-Control","no-cache");

    QNetworkReply *reply = manager->post(requestauthorization,data);

    reply->waitForReadyRead(200);


    QByteArray bytes = reply->readAll(); // bytes

    QString stringreply = QString::fromUtf8(bytes);
    qDebug() << "Server reply "<< stringreply;

    if (reply->errorString() != "Unknown error"){
        qDebug() << "API REQUEST ERROR "<< reply->errorString();
    }

    // read and parse reply


    QJsonDocument loadDoc(QJsonDocument::fromJson(bytes));
    QJsonObject jsonObject = loadDoc.object();
    QString code =  jsonObject["code"].toString();



    if (!code.isEmpty()){
        qDebug() << "code" << code;
        return code;
    }

    return "z";
}

