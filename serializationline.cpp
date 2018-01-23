#include "serializationline.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

SerializationLine::SerializationLine(QObject *parent) : QObject(parent)
{
}

SerializationLine::SerializationLine(QString TCPAddress, quint16 TCPport, quint16 linespeed, quint16 countinminute)
{
        Socket = new QTcpSocket(this);

        // коннекшн если готов читать
        connect(Socket,SIGNAL(readyRead()),this,SLOT(serverRead()));

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

void SerializationLine::SetMedicamentOptions(QString preparatname, QString gtin, QString experyDate, QString batchName)
{
    QJsonObject mainJsonObject;
    QJsonArray Parametrs;
    QJsonObject jPreparatName;
    QJsonObject jGTIN;
    QJsonObject jExperyDate;
    QJsonObject jBatchName;

    Parametrs.append(jPreparatName);
    Parametrs.append(jGTIN);
    Parametrs.append(jExperyDate);
    Parametrs.append(jBatchName);

    mainJsonObject["jPreparatName"] = preparatname;
    mainJsonObject["jGTIN"] = gtin;
    mainJsonObject["jExperyDate"] = experyDate;
    mainJsonObject["jBatchName"] = batchName;

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

void SerializationLine::serverRead()
{
    QByteArray ba;

    //если есть что читать из ТСР, то читаем все байты
    while(Socket->bytesAvailable()>0)
    {
        ba = Socket->readAll();
        //Складываем их в общий байтмассив
        // ba.append(ba);
        //ClientDataRead = QTextCodec::codecForMib(106)->toUnicode(ba);
        qDebug() <<  ba;
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(ba));
    QJsonObject jsonObject = loadDoc.object();

    QString response =  jsonObject["Response"].toString();


    if (!response.isEmpty())
    {
        emit ResponseRecieved(getTCPAddress(), getTCPPort(),response);

        qDebug() << response;
        qDebug() << getTCPAddress();
        qDebug() << getTCPPort();
    }

    return;
}
