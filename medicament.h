#ifndef MEDICAMENT_H
#define MEDICAMENT_H

#include <QObject>

class medicament : public QObject
{
    Q_OBJECT
public:
    explicit medicament(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MEDICAMENT_H