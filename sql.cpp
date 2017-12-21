#include "sql.h"
#include "QtSql"

SQL::SQL()
{

    baseConnection();
}

SQL::SQL(QString path)
{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName(path);

//    QString hostname = "LOCALHOST\\MySQL57";
    QString dbname = "mark";
    QString hostname = "192.168.1.37";
    int port = 3306;

    QString user = "markirovka";
    QString password = "WD8NHWq3T0zT";
    QString driver = "QMYSQL";

    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, QString("drivername"));

    db.setDatabaseName(dbname);
    db.setHostName(hostname);
    db.setPort(port);
    if (!db.open(user, password)) {
        err = db.lastError();
        qDebug() << "NOT opened!";
    }
    else
    {
        qDebug() << "Base opened!";
    }

//    qDebug() << sel("company_name", "Company", "company = 'BFZ' ","company_name")[0];
//    qDebug() << sel("company_name", "Company", "company = 'KORVAS' ","company_name")[0];
}

void SQL::baseConnection()
{
    QString path  = "C:/Work/SQL/mydatabase";
    //    QString path  = "C:/Work/SQL/DB";
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
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
    QSqlQuery query;
    //Задаем запрос
    QString execc = "SELECT " + select + " FROM " + from;

    //    qDebug() << execc;
    if (where != "")
    {
        execc += " WHERE " + where;
    }

    //     qDebug() << execc;


    if (!query.exec(execc)) {
        qDebug() << "Unable to execute query - exiting";

    }
    else
    {
        //        qDebug() << "Exec success";
    }
    //Reading of the data
    QSqlRecord SQLrec     = query.record();
    QStringList    strName;

    while (query.next()) {
        strName.append( query.value(SQLrec.indexOf(rec)).toString() );
    }

    if (strName.length() == 0)
        strName.append(""); // зачем?

    return strName;
}

QSqlError SQL::makesqlreq(QString req)
{
    QSqlQuery query;
    //Задаем запрос
    QString execc = req;

    if (!query.exec(execc)) {
        qDebug() << "Unable to execute query - exiting2";
        qDebug() << query.lastError();

        //        sendmessage(query.lastQuery());
    }


    return query.lastError() ;

}

void SQL::upd(QString u, QString s, QString w)
{
    QString qstr = "UPDATE " + u + " SET " + s + " WHERE " + w;
    QSqlQuery query;
    if (!query.exec(qstr)) {
        qDebug() << "Unable to inser operation";
        //return 1;
    }
}
