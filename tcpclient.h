#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TCPCLIENT_H