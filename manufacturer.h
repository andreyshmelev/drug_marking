#ifndef MANUFACTURER_H
#define MANUFACTURER_H

#include <QObject>

class manufacturer
{
public:

    manufacturer();     // конструктор в котором подгружаем все данные производителя
    manufacturer(QString subject_idc, QString organisation_namec, QString emailc, QString ulc, QString flc, QString innc, QString kppc, QString owneridc);     // конструктор в котором подгружаем все данные производителя
    QString get_subject_id();           // Идентификатор субъекта обращения в ИС "Маркировка товаров"
    QString get_owner_id();             // Идентификатор собственника в случае контрактного производства
    QString get_organisation_name();    // Наименование организации
    QString get_email();// email
    QString get_ul();   //Идентификационный номер налогоплательщика - юридического лица
    QString get_fl();   //Идентификационный номер налогоплательщика - физического лица
    QString get_inn();  //Идентификационный номер налогоплательщика
    QString get_kpp();  //Код причины постановки на учет

private: // methods

    // считываем эти параметры из файла или базы данных предприятия
    void set_subject_id(QString new_subject_id);
    void set_company_name(QString new_organisation_name);
    void set_email(QString new_email);
    void set_ul(QString new_ul);
    void set_fl(QString new_fl);
    void set_inn(QString new_inn);
    void set_kpp(QString new_kpp);
    void set_ownerid(QString new_ownerid);

private: // variables

    QString company_name;
    QString ul;
    QString fl;
    QString inn;
    QString kpp;
    QString ownerid;
    QString email;
    QString subject_id;
};

#endif // MANUFACTURER_H
