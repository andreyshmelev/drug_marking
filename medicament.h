#ifndef MEDICAMENT_H
#define MEDICAMENT_H

#include <QObject>


class medicament : public QObject
{
    Q_OBJECT

public:
    explicit medicament(QObject *parent = 0 );
    medicament (QString medicament_name, QString GTIN, QString SerialNumber, QString BatchNumber, QString ExperyDate, QString sGTIN, QString TNVED);     // конструктор в котором подгружаем все данные медикамента
signals:

public:
    QString medicament_name;
    QString GTIN;
    QString SerialNumber;
    QString BatchNumber;
    QString ExperyDate;
    QString sGTIN;
    QString TNVED;
    QString condidions;

public slots:
};

#endif // MEDICAMENT_H
