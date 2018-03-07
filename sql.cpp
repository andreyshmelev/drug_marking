#include "sql.h"
#include "QtSql"
#include <QObject>



SQL::SQL(QString dbname, QString hostname, quint16 port, QString user, QString password)
{


    QString driver = "QMYSQL";

    QSqlError err;
    db = QSqlDatabase::addDatabase(driver, QString("drivername"));

    db.setDatabaseName(dbname);
    db.setHostName(hostname);
    db.setPort(port);
    if (!db.open(user, password)) {
        err = db.lastError();

        qDebug() << "Faaaaaaaiiiiiillllllllllllllllllllllllll";
        databaseErrorSignal("Fail");
        return;
    }
    else
    {
        databaseErrorSignal("Ok");
    }

}

void SQL::baseConnection()
{
    QString path  = "C:/Work/SQL/mydatabase";
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(path);
        db.setUserName("korvas");
        db.setHostName("localhost");
        db.setPassword("password");
        if (!db.open()) {
            databaseErrorSignal("Fail");
            return;
        }
        else
        {
            databaseErrorSignal("Ok");
        }
    }
}

QStringList SQL::sel(QString select, QString from, QString where, QString rec)
{
    QSqlQuery query("", db);
    //Задаем запрос
    QString execc = "SELECT " + select + " FROM " + from;

    if (where != "")
    {
        execc += " WHERE " + where;
    }

    if (!query.exec(execc)) {
        databaseErrorSignal("Fail");

        return QStringList("");
    }
    else
    {
        databaseErrorSignal("Ok");
    }

    //Reading of the data
    QSqlRecord SQLrec     = query.record();
    QStringList    strName;

    while (query.next()) {
        strName.append( query.value(SQLrec.indexOf(rec)).toString() );
    }

    if (strName.length() == 0)
        strName.append("");

    return strName;
}

QStringList SQL::getsumm(QString select, QString from, QString where, QString rec)
{
    QSqlQuery query("", db);
    //Задаем запрос
    QString execc = "SELECT " + select + " FROM " + from;

    if (where != "")
    {
        execc += " WHERE " + where;
    }

    if (!query.exec(execc)) {
        databaseErrorSignal("Fail");
        return QStringList("");
    }
    else
    {
        databaseErrorSignal("Ok");
    }

    QSqlRecord SQLrec     = query.record();
    QStringList    strName;

    while (query.next()) {
        strName.append( query.value(SQLrec.indexOf(rec)).toString() );
    }

    if (strName.length() == 0)
        strName.append("");

    return strName;
}

QStringList SQL::seldistinct(QString select, QString from, QString where, QString rec)
{
    QSqlQuery query("", db);
    //Задаем запрос
    QString execc = "SELECT DISTINCT " + select + " FROM " + from;

    if (where != "")
    {
        execc += " WHERE " + where;
    }

    if (!query.exec(execc)) {
        databaseErrorSignal("Fail");
        return QStringList("");
    }
    else
    {
        databaseErrorSignal("Ok");
    }

    //Reading of the data
    QSqlRecord SQLrec     = query.record();
    QStringList    strName;

    while (query.next()) {
        strName.append( query.value(SQLrec.indexOf(rec)).toString() );
    }

    if (strName.length() == 0)
        strName.append("");

    return strName;
}

void SQL::makesqlreq(QString req)
{
    QSqlQuery query("", db);
    //Задаем запрос
    QString execc = req;

    if (!query.exec(execc)) {
        databaseErrorSignal("Fail");
        return;
    }
    else
    {
        databaseErrorSignal("Ok");
    }
}

void SQL::upd(QString u, QString s, QString w)
{
    QString qstr = "UPDATE " + u + " SET " + s + " WHERE " + w;
    QSqlQuery query;
    if (!query.exec(qstr)) {
        databaseErrorSignal("Fail");
        return;
    }
    else
    {
        databaseErrorSignal("Ok");
    }
}
