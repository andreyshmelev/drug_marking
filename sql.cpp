#include "sql.h"
#include "QtSql"

SQL::SQL()
{
  //  void baseConnection()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("DB");
        db.setUserName("korvas");
        db.setHostName("localhost");
        db.setPassword("password");
        if (!db.open()) {
            qDebug() << "Cannot open database:" << db.lastError();

        }
    }

//    QStringList sel(QString select, QString from, QString where, QString rec)


    {
        QSqlQuery query;
        //Задаем запрос
        QString execc = "SELECT rowid, * FROM \"drug_old\" ";


        if (!query.exec(execc)) {
            qDebug() << "Unable to execute query - exiting";

        }
        else
        {
            qDebug() << "Exec success";
        }

        int t = 0;
        //Reading of the data
        QSqlRecord SQLrec     = query.record();
        QStringList    strName;

        while (query.next()) {
            strName  += query.value(t++).toString();
            qDebug() << strName ;
        }
   }

}
