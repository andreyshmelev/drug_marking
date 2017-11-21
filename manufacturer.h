#ifndef MANUFACTURER_H
#define MANUFACTURER_H

#include <QObject>

class manufacturer
{
public:

    manufacturer();
    QString get_subject_id();           // Идентификатор субъекта обращения в ИС "Маркировка товаров"
    QString get_organisation_name();    // Наименование организации
    QString get_email();// email
    QString get_ul();   //Идентификационный номер налогоплательщика - юридического лица
    QString get_fl();   //Идентификационный номер налогоплательщика - физического лица
    QString get_inn();  //Идентификационный номер налогоплательщика
    QString get_kpp();  //Код причины постановки на учет

private: // methods

    // считываем эти параметры из файла или базы данных предприятия
    void set_subject_id(QString new_subject_id);
    void set_organisation_name(QString new_organisation_name);
    void set_email(QString new_email);
    void set_ul(QString new_ul);
    void set_fl(QString new_fl);
    void set_inn(QString new_inn);
    void set_kpp(QString new_kpp);

private: // variables

    QString email;
    QString organisation_name;
    QString subject_id;
    QString ul;
    QString fl;
    QString inn;
    QString kpp;
};

#endif // MANUFACTURER_H
