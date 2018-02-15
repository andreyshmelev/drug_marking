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

private:
    QNetworkAccessManager *manager ;

    QString code;
    QString token;

public slots:
    void replyfinished(QNetworkReply *reply);
    QString GetCodeAuth();
    void GetDocumentsList(QString code, QString filename);
    void GetCurrentUser(QString code);
    void Logout(QString code);
    void AskToken(QString code);
};

#endif // APIMARK_H
