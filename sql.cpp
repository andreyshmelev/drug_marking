#include "sql.h"
#include "QtSql"

SQL::SQL()
{
    baseConnection();
}

SQL::SQL(QString path)
{
    QString dbname = "mark";
    QString hostname = "192.168.1.63";  // <<-"localhost"; //"192.168.1.62";127.0.0.1
    int port = 3306;
    QString user = "markirovka";
    QString password = "WD8NHWq3T0zT";
//    qDebug() << QSqlDatabase::drivers();


    QString driver = "QMYSQL";

    QSqlError err;
    db = QSqlDatabase::addDatabase(driver, QString("drivername"));

    db.setDatabaseName(dbname);
    db.setHostName(hostname);
    db.setPort(port);
    if (!db.open(user, password)) {
        err = db.lastError();
    }
    else
    {
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
            qDebug() << "Cannot open database:" << db.lastError();
        }
        else
            qDebug() << "Connect sucess" ;
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
        qDebug() << "Last DataBase Error" << query.lastError();
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
        //        qDebug() << "Unable to execute query - exiting";
        qDebug() << "Last DataBase Error" << query.lastError();
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
        qDebug() << "Last DataBase Error" << query.lastError();
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

QSqlError SQL::makesqlreq(QString req)
{
    QSqlQuery query("", db);
    //Задаем запрос
    QString execc = req;

    if (!query.exec(execc)) {
        qDebug() << query.lastError();
    }
    return query.lastError() ;
}

void SQL::upd(QString u, QString s, QString w)
{
    QString qstr = "UPDATE " + u + " SET " + s + " WHERE " + w;
    QSqlQuery query;
    if (!query.exec(qstr)) {
        qDebug() << "Last DataBase Error" << query.lastError();
    }
}
