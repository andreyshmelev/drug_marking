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

signals:

private:
    QNetworkAccessManager *manager ;

    QNetworkRequest requestauthorization;

public slots:
    void replyfinished(QNetworkReply *reply);
    QString GetCodeAuth();
};

#endif // APIMARK_H
