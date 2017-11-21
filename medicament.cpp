#include "medicament.h"

medicament::medicament(QObject *parent) : QObject(parent)
{

}

medicament::medicament(QString medicament_name, QString GTIN, QString SerialNumber, QString BatchNumber, QString ExperienceDate, QString sGTIN, QString TNVED)
{
this->medicament_name = medicament_name;
    this->GTIN = GTIN;
    this->SerialNumber = SerialNumber;
    this->BatchNumber = BatchNumber;
    this->ExperienceDate = ExperienceDate;
    this->sGTIN = sGTIN;
    this->TNVED = TNVED;
}
