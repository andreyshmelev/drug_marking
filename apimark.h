#ifndef APIMARK_H
#define APIMARK_H

#include <QObject>

class APIMARK : public QObject
{
    Q_OBJECT
public:
    explicit APIMARK(QObject *parent = nullptr);

signals:

public slots:
};

#endif // APIMARK_H