#include "pack.h"

pack::pack(QObject *parent) : QObject(parent)
{

}

QString pack::getSSCC() const
{
    return SSCC;
}

void pack::setSSCC(const QString &value)
{
    SSCC = value;
}

quint16 pack::getCountdrugsmax() const
{
    return countdrugsmax;
}

void pack::setCountdrugsmax(const quint16 &value)
{
    countdrugsmax = value;
}

quint16 pack::getCountdrugscurrent() const
{
    return countdrugscurrent;
}

void pack::setCountdrugscurrent(const quint16 &value)
{
    countdrugscurrent = value;
}

quint16 pack::getCountlayersmax() const
{
    return countlayersmax;
}

void pack::setCountlayersmax(const quint16 &value)
{
    countlayersmax = value;
}

quint16 pack::getCountdrugsinlayers() const
{
    return countdrugsinlayers;
}

void pack::setCountdrugsinlayers(const quint16 &value)
{
    countdrugsinlayers = value;
}
