#ifndef MEDICAMENT_H
#define MEDICAMENT_H

#include <QObject>

class medicament : public QObject
{
    Q_OBJECT
public:
    explicit medicament(QObject *parent = nullptr);
    medicament (QString medicament_name, QString GTIN, QString SerialNumber, QString BatchNumber, QString ExperienceDate, QString sGTIN, QString TNVED);     // конструктор в котором подгружаем все данные медикамента
signals:

public:
    QString medicament_name;
    QString GTIN;
    QString SerialNumber;
    QString BatchNumber;
    QString ExperienceDate;
    QString sGTIN;
    QString TNVED;

public slots:
};

#endif // MEDICAMENT_H
