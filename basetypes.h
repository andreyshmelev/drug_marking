#ifndef BASETYPES_H
#define BASETYPES_H

enum ControlSamplesTypeEnum
  {
     ToQualityControl = 1,  // отбор контрольных образцов в рамках процесса контроля качества
     ToVerifyCompliance = 3 // отбор образцов для подтверждения соответствия в рамах технического регулирования
  };

enum ActionIDTypeEnum
  {
     RegisterEndPacking = 311,      // регистрация в ИС «Маркировка» сведений о завершении этапа окончательной упаковки
     RegisterControlSamples = 312,  // регистрация в ИС «Маркировка» сведений об отборе образцов лекарственных препаратов
     RegisterProductEmission = 313  // регистрация в ИС «Маркировка» сведений о выпуске готовой продукции
  };


enum OrderTypeEnum
  {
    OwnProduction = 1 , // Собственное производство
    ContractProduction = 2   // Контрактное производствоs
  };



#endif // BASETYPES_H

