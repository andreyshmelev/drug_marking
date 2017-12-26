#ifndef BASEBUISNESSCLASS_H
#define BASEBUISNESSCLASS_H

#include <QObject>

class BaseBuisnessClass : public QObject
{
    Q_OBJECT
public:
    explicit BaseBuisnessClass(QObject *parent = 0);



signals:

    void RegistrationStarted();
    void StopRegisterControlSamples();
    void RegistrationToggled();
//    void RegistrationCompleted(QList<medicament *> MedList , quint8 controlsamplestype);
};

#endif // BASEBUISNESSCLASS_H
