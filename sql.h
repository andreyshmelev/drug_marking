#ifndef SQL_H
#define SQL_H

#include <QtSql>
#include <QObject>

class SQL : public QObject
{
public:
    SQL();
    SQL(QString path);
    QSqlDatabase db;
public slots:
    void baseConnection();
    QStringList sel(QString select, QString from, QString where, QString rec);
    QStringList getsumm(QString select, QString from, QString where, QString rec);

    QStringList seldistinct(QString select, QString from, QString where, QString rec);
    QSqlError makesqlreq(QString req);
    void upd(QString u, QString s, QString w);
};

#endif // SQL_H
