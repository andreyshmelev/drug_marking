#ifndef APIMARK_H
#define APIMARK_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>


class APIMARK : public QObject
{
    Q_OBJECT
public:
    explicit APIMARK(QObject *parent = nullptr);

    QString getCode() const;
    void setCode(const QString &value);

    QString getToken() const;
    void setToken(const QString &value);

signals:
    void message(QString mes);

private:
    QNetworkAccessManager *manager ;

    QString code;
    QString token;

public slots:
    void replyfinished(QNetworkReply *reply);
    QString GetCodeAuth();
    void GetOutcomeDocumentsList(QString token);
    void GetCurrentUser(QString code);
    void Logout(QString code);
    void AskToken(QString code);
    void Sendfile(QString token, QString filename);
    void RegisterNonResidentUser(QString token);
    void RegisterResidentUser(QString token);
    void GetIncomeDocumentsList();
    void DownloadDocumentByID(QString docID);
private slots:
    QString generateRandomStringForGUID(int lenght);
    QString generateGUIDString();
};

#endif // APIMARK_H
