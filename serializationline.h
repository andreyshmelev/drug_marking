#ifndef SERIALIZATIONLINE_H
#define SERIALIZATIONLINE_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

class SerializationLine : public QObject
{
    Q_OBJECT
public:
    explicit SerializationLine(QObject *parent = 0);

    SerializationLine(QString TCPAddress,quint16 TCPport,quint16 linespeed, quint16 countinminute);

    QString getTCPAddress() const;
    void setTCPAddress(const QString &value);

    quint16 getTCPPort() const;
    void setTCPPort(const quint16 &value);

    quint16 getSpeedmmsec() const;
    void setSpeedmmsec(const quint16 &value);

    quint16 getCountinminute() const;
    void setCountinminute(const quint16 &value);


    QTcpSocket *Socket ;

signals:

public slots:

private slots:
    void connectTcp(QString address, quint16 port);
private:

QString TCPAddress;
quint16 port;
quint16 speedmmsec;
quint16 countinminute;

};

#endif // SERIALIZATIONLINE_H
