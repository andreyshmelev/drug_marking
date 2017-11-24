#include "manufacturer.h"

manufacturer::manufacturer()
{
    //    set_subject_id( "(343)374–35–66" );
    //    set_organisation_name("ЗАО \"Берёзовский фармацевтический завод\"");
}

manufacturer::manufacturer(QString subject_idc, QString organisation_namec, QString emailc, QString ulc, QString flc, QString innc, QString kppc, QString owneridc)
{
    set_subject_id( subject_idc );
    set_company_name(organisation_namec);
    set_email(emailc);
    set_ul(ulc);
    set_fl(flc);
    set_inn(innc);
    set_kpp(kppc);
    set_ownerid(owneridc);
}

QString manufacturer::get_subject_id()
{
    return subject_id;
}

QString manufacturer::get_owner_id()
{
    return ownerid;
}

QString manufacturer::get_organisation_name()
{
    return company_name;
}

QString manufacturer::get_email()
{
    return email;
}

QString manufacturer::get_ul()
{
    return ul;
}

QString manufacturer::get_fl()
{
    return fl;
}

QString manufacturer::get_inn()
{
    return inn;
}

QString manufacturer::get_kpp()
{
    return kpp;
}

void manufacturer::set_subject_id(QString new_subject_id)
{
    subject_id = new_subject_id;
}

void manufacturer::set_company_name(QString new_organisation_name)
{
    company_name = new_organisation_name;
}

void manufacturer::set_email(QString new_email)
{
    email = new_email;
}

void manufacturer::set_ul(QString new_ul)
{
    ul = new_ul ;
}

void manufacturer::set_fl(QString new_fl)
{
    fl = new_fl;
}

void manufacturer::set_inn(QString new_inn)
{
    inn = new_inn;
}

void manufacturer::set_kpp(QString new_kpp)
{
    kpp = new_kpp;
}

void manufacturer::set_ownerid(QString new_ownerid)
{
    ownerid = new_ownerid;
}
