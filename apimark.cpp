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
    QNetworkRequest requestauthorization;
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyfinished(QNetworkReply*)));

    QByteArray data = ("{\"client_id\": \"ef77a1f8-e374-451d-9da9-7c3519d0d143\",\"client_secret\": \"c4bf1684-eb4e-4119-bed7-b28fc3beb68b\",\"user_id\": \"test_non_resident\",\"auth_type\": \"PASSWORD\"}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/auth"); // авторизируемся туточки

    requestauthorization.setUrl(serviceURL);
    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Cache-Control","no-cache");

    QNetworkReply *reply = manager->post(requestauthorization,data);
}

QString APIMARK::getCode() const
{
    return code;
}

void APIMARK::setCode(const QString &value)
{
    qDebug() << "code now is" << value;
    code = value;
}

QString APIMARK::getToken() const
{
    return token;
}

void APIMARK::setToken(const QString &value)
{
    qDebug() << "token now is" << value;
    token = value;
}

void APIMARK::replyfinished(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll(); // bytes
    QString stringreply = QString::fromUtf8(bytes);

    if (reply->errorString() != "Unknown error"){
        qDebug() << "API REQUEST ERROR "<< reply->errorString();
        emit message("API REQUEST ERROR " + reply->errorString() + ", "  + stringreply);
    }

    // read and parse reply
    QJsonDocument loadDoc(QJsonDocument::fromJson(bytes));
    QJsonObject jsonObject = loadDoc.object();
    QString code =  jsonObject["code"].toString();
    QString token =  jsonObject["token"].toString();

    if (!code.isEmpty()){
        setCode(code);
        emit message("code: " + code);
    }

    if (!token.isEmpty()){
        setToken(token);
        emit message("token: " + token);
    }
}


QString APIMARK::GetCodeAuth()
{
    QNetworkRequest requestauthorization;
    QByteArray data = ("{\"client_id\": \"ef77a1f8-e374-451d-9da9-7c3519d0d143\",\"client_secret\": \"c4bf1684-eb4e-4119-bed7-b28fc3beb68b\",\"user_id\": \"test_non_resident\",\"auth_type\": \"PASSWORD\"}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/auth"); // авторизируемся туточки

    requestauthorization.setUrl(serviceURL);
    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Cache-Control","no-cache");

    QNetworkReply *reply = manager->post(requestauthorization,data);


//    while

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
        setCode(code);
        return code;
    }

    return "z";
}

void APIMARK::GetDocumentsList(QString token, QString filename)
{

//    manager = new QNetworkAccessManager(this);
    QNetworkRequest requestauthorization;
//    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyfinished(QNetworkReply*)));

    QByteArray data = ("{\"filter\": {\"doc_status\": \"PROCESSED_DOCUMENT\"},\"start_from\": 0,\"count\": 100}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/documents/outcome"); // отправка файла XML

    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));


//    qDebug() << "SendDocument tokenbyte "<< tokenbyte;

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data);
}

void APIMARK::GetCurrentUser(QString token)
{
//    manager = new QNetworkAccessManager(this);
    QNetworkRequest requestauthorization;

    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/users/current"); // отправка файла XML

    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));

    QByteArray data0 ;
    data0.clear();

//    qDebug() << "GetCurrentUser tokenbyte "<< tokenbyte;

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->get(requestauthorization);
}


void APIMARK::Logout(QString token)
{
//    manager = new QNetworkAccessManager(this);
    QNetworkRequest requestauthorization;

    QByteArray data = ("{\"filter\": {\"doc_status\": \"PROCESSED_DOCUMENT\"},\"start_from\": 0,\"count\": 100}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/logout"); // отправка файла XML

    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));

    QByteArray data0 ;
    data0.clear();

    qDebug() << "GetCurrentUser tokenbyte "<< tokenbyte;

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data0);
}



void APIMARK::AskToken(QString code)
{

//    manager = new QNetworkAccessManager(this);
    QNetworkRequest requestauthorization;
//    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyfinished(QNetworkReply*)));

    QString stringdata = QString("{\"code\": \"%1\",\"password\" : \"password\"}").arg(code);
    QByteArray data ;

    data.clear();
    data.append(stringdata);

    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/token"); // отправка файла XML

    requestauthorization.setUrl(serviceURL);


    requestauthorization.setRawHeader("Content-Type","application/json");

    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data);

    qDebug() << "AskToken";
}

