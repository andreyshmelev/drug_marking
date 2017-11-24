#include "sql.h"
#include "QtSql"

SQL::SQL()
{

    baseConnection();
}

SQL::SQL(QString path)
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.setUserName("korvas");
    db.setHostName("localhost");
    db.setPassword("password");
    if (!db.open()) {
        qDebug() << "Cannot open database:" << db.lastError();
    }

    qDebug() << sel("company_name", "Company", "company = 'BFZ' ","company_name")[0];
    qDebug() << sel("company_name", "Company", "company = 'KORVAS' ","company_name")[0];

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
    strName += ""; // зачем?

//    qDebug() << strName << ";\t";


    return strName;
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
