#include "serializationline.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

SerializationLine::SerializationLine(QObject *parent) : QObject(parent)
{

}

SerializationLine::SerializationLine(QString TCPAddress, quint16 TCPport, quint16 linespeed, quint16 countinminute)
{

    setTCPAddress(TCPAddress);
    setTCPPort(TCPport);
    setSpeedmmsec(linespeed);
    setCountinminute(countinminute);
    connectTcp(getTCPAddress(),getTCPPort());

    SetFizikalOptions(linespeed, countinminute);
}

void  SerializationLine::SetFizikalOptions(quint16 linespeed, quint16 countinminute)
{
    QJsonObject mainJsonObject;
    QJsonArray Parametrs;
    QJsonObject jObject;
    QJsonObject jLineSpeed;
    QJsonObject jCountMinute;

    Parametrs.append(jLineSpeed);
    Parametrs.append(jCountMinute);

    mainJsonObject["jLineSpeed"] = countinminute;
    mainJsonObject["jCountMinute"] = linespeed;

    QJsonDocument Doc(mainJsonObject);
    QByteArray ba = Doc.toJson();

    Socket->write(ba);
    qDebug() << ba;
}

QString SerializationLine::getTCPAddress() const
{
    return TCPAddress;
}

void SerializationLine::setTCPAddress(const QString &value)
{
    TCPAddress = value;
}

quint16 SerializationLine::getTCPPort() const
{
    return port;
}

void SerializationLine::setTCPPort(const quint16 &value)
{
    port = value;
}

quint16 SerializationLine::getSpeedmmsec() const
{
    return speedmmsec;
}

void SerializationLine::setSpeedmmsec(const quint16 &value)
{
    speedmmsec = value;
}

quint16 SerializationLine::getCountinminute() const
{
    return countinminute;
}

void SerializationLine::setCountinminute(const quint16 &value)
{
    countinminute = value;
}

void SerializationLine::connectTcp(QString address, quint16 port)
{
    Socket = new QTcpSocket(this);
    Socket->connectToHost(address, port);
    Socket->waitForConnected(100);

    qDebug() << address << port;
}

void SerializationLine::SendTcpData(QString data)
{

}

void SerializationLine::SendTcpData(QByteArray data)
{

}
