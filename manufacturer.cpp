#include "manufacturer.h"

manufacturer::manufacturer()
{

}

QString manufacturer::get_subject_id()
{
    return subject_id;
}

QString manufacturer::get_organisation_name()
{
    return organisation_name;
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

void manufacturer::set_organisation_name(QString new_organisation_name)
{
    organisation_name = new_organisation_name;
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