#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

#include "apimark.h"

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
    code = value;
}

QString APIMARK::getToken() const
{
    return token;
}

void APIMARK::setToken(const QString &value)
{
    token = value;
}

QString APIMARK::getFileDownloadLink() const
{
    return FileDownloadLink;
}

void APIMARK::setFileDownloadLink(const QString &value)
{
    FileDownloadLink = value;
}

void APIMARK::replyfinished(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll(); // bytes
    QString stringreply = QString::fromUtf8(bytes);

    if (reply->errorString() != "Unknown error"){
        emit message("API REQUEST ERROR " + reply->errorString() + ", "  + stringreply);
    }

    // read and parse reply
    QJsonDocument loadDoc(QJsonDocument::fromJson(bytes));
    QJsonObject jsonObject = loadDoc.object();
    QString code =  jsonObject["code"].toString();
    QString token =  jsonObject["token"].toString();
    QString user =  jsonObject["user"].toString();
    QString filelink =  jsonObject["link"].toString();

    if (!code.isEmpty()){
        setCode(code);
        emit message("Код: " + code);
        return;
    }

    if (!token.isEmpty()){
        setToken(token);
        emit message("Токен: " + token);
        return;
    }

    if (!user.isEmpty()){
        emit message("Пользователь: " + user);
        return;
    }

    if (!filelink.isEmpty()){
        emit message("Ссылка на скачивание: " + filelink);
        setFileDownloadLink(filelink);
        return;
    }

    emit message(stringreply);
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

void APIMARK::GetOutcomeDocumentsList(QString token)
{
    QNetworkRequest requestauthorization;
    QByteArray data = ("{\"filter\": {},\"start_from\": 0,\"count\": 1000}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/documents/outcome");

    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data);
}

void APIMARK::GetIncomeDocumentsList()
{
    QNetworkRequest requestauthorization;
    QByteArray data = ("{\"filter\": {},\"start_from\": 0,\"count\": 1000}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/documents/income");
    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(getToken()));
    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data);
}

void APIMARK::GetCurrentUser(QString token)
{
    //    manager = new QNetworkAccessManager(this);
    QNetworkRequest requestauthorization;

    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/users/current");

    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));

    QByteArray data0 ;
    data0.clear();
    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->get(requestauthorization);
}


void APIMARK::Logout(QString token)
{
    QNetworkRequest requestauthorization;

    QByteArray data = ("{\"filter\": {\"doc_status\": \"PROCESSED_DOCUMENT\"},\"start_from\": 0,\"count\": 100}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/logout"); // отправка файла XML

    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));

    QByteArray data0 ;
    data0.clear();

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data0);
}

void APIMARK::AskToken(QString code)
{
    QNetworkRequest requestauthorization;

    QString stringdata = QString("{\"code\": \"%1\",\"password\" : \"password\"}").arg(code);
    QByteArray data ;

    data.clear();
    data.append(stringdata);

    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/token"); // отправка файла XML

    requestauthorization.setUrl(serviceURL);
    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data);
}

QString APIMARK::generateRandomStringForGUID(int lenght)
{
    const QString possibleCharacters("abcdef0123456789");  // abcdefghijklmnopqrstuvwxyz
    const int randomStringLength = lenght; // SNlenght  assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }

    QString newrandomstring = randomString;
    return newrandomstring;
}


QString APIMARK::generateGUIDString()
{
    QString GIDString = generateRandomStringForGUID(8)+"-" + generateRandomStringForGUID(4)+"-" + generateRandomStringForGUID(4)+"-" + generateRandomStringForGUID(4)+"-" + generateRandomStringForGUID(12);
    return GIDString;
}

void APIMARK::Sendfile(QString token, QString filename, int doctype)
{
    QNetworkRequest requestauthorization;
    ////////
    QFile file(filename);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&file);
    QString filecontent = in.readAll();
    QByteArray bytecontentinRFC2045;
    bytecontentinRFC2045.append(filecontent);
    QString stringcontentinRFC2045;
    stringcontentinRFC2045.append( bytecontentinRFC2045.toBase64() );
    QByteArray byteCodeRFC2045;
    byteCodeRFC2045.append(getCode());
    QString CodeIn2045 = byteCodeRFC2045.toBase64();

    ////////

    QJsonObject mainJsonObject;
    mainJsonObject["doc_type"] = doctype;
    mainJsonObject["document"] = stringcontentinRFC2045;
    mainJsonObject["sign"] = "";
    mainJsonObject["request_id"] = generateGUIDString();

    qDebug() << "doc_type" <<  mainJsonObject["doc_type"] ;

    QJsonDocument Doc(mainJsonObject);
    QByteArray ba = Doc.toJson();

    ////////

    QByteArray data = ba.simplified();
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/documents/send"); // отправка файла XML
    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));
    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data);
}


void APIMARK::RegisterNonResidentUser(QString token)
{
    QNetworkRequest requestauthorization;
    QByteArray data = ("{\"sys_id\" : \"6be50ba4-c20c-4b90-90a4-c6edbb97fe06\",\"username\" : \"korvas\",\"password\" : \"password123\",\"first_name\" : \"Андрей\",\"last_name\" : \"Шмелев\"}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/registration/user_nonresident"); // отправка файла XML

    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data);
}

void APIMARK::RegisterResidentUser(QString token)
{
    QNetworkRequest requestauthorization;

    QByteArray data = ("{\"sys_id\" : \"6be50ba4-c20c-4b90-90a4-c6edbb97fe06\",  \"public_cert\" : \"\"   ,\"username\" : \"korvas2\",\"password\" : \"password123\",\"first_name\" : \"Андрей\",\"last_name\" : \"Шмелев\"}");
    QUrl serviceURL("http://dev-api.markirovka.nalog.ru/api/v1/registration/user_resident"); // отправка файла XML
    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(token));

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->post(requestauthorization,data);
}

void APIMARK::GetDownloadLinkDocumentByID(QString docID)
{
    QNetworkRequest requestauthorization;

    QByteArray data = ("{\"group_name\" : \"Тестовая группа для ООО Пилюльки\",\"rights\" : [\"DOWNLOAD_DOCUMENT\"]}");
    QString URLstr = QString("http://dev-api.markirovka.nalog.ru/api/v1/documents/download/%1").arg(docID);

    QUrl serviceURL(URLstr);
    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(getToken()));

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->get(requestauthorization);
}

void APIMARK::GetRules()
{
    QNetworkRequest requestauthorization;
    QString URLstr = QString("http://dev-api.markirovka.nalog.ru/api/v1/rights/about");

    QUrl serviceURL(URLstr);
    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(getToken()));

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->get(requestauthorization);
}

void APIMARK::DownloadDocumentByLink(QString link, QString filename)
{
    QNetworkRequest requestauthorization;
    QString URLstr = QString(link);

    QUrl serviceURL(URLstr);
    requestauthorization.setUrl(serviceURL);
    QByteArray tokenbyte ;
    tokenbyte.append(QString("token %1").arg(getToken()));

    requestauthorization.setRawHeader("Content-Type","application/json");
    requestauthorization.setRawHeader("Authorization",tokenbyte);
    requestauthorization.setRawHeader("Cache-Control","no-cache");
    QNetworkReply *reply = manager->get(requestauthorization);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

     if(reply)
    {
      qDebug() << reply->readAll();
    }
}
