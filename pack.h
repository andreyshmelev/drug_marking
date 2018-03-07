#ifndef PACK_H
#define PACK_H

#include <QObject>

class pack : public QObject
{
    Q_OBJECT
public:
    explicit pack(QObject *parent = 0);

    QString getSSCC() const;
    void setSSCC(const QString &value);

    quint16 getCountdrugsmax() const;
    void setCountdrugsmax(const quint16 &value);

    quint16 getCountdrugscurrent() const;
    void setCountdrugscurrent(const quint16 &value);

    quint16 getCountlayersmax() const;
    void setCountlayersmax(const quint16 &value);

    quint16 getCountdrugsinlayers() const;
    void setCountdrugsinlayers(const quint16 &value);

signals:

public slots:

private:
    QString SSCC;
    quint16 countdrugsmax;
    quint16 countdrugscurrent;
    quint16 countlayersmax;
    quint16 countdrugsinlayers;


};

#endif // PACK_H
