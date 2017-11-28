#ifndef SQL_H
#define SQL_H

#include <QtSql>
#include <QObject>

class SQL : public QObject
{
public:
    SQL();
    SQL(QString path);
public slots:
    void baseConnection();
    QStringList sel(QString select, QString from, QString where, QString rec);
    void makesqlreq(QString req);
    void upd(QString u, QString s, QString w);

private:
    QSqlQuery query;
    //Задаем запрос
    QString execc;
};

#endif // SQL_H