#ifndef BASETYPES_H
#define BASETYPES_H

enum ControlSamplesTypeEnum
  {
     ToQualityControl = 1,  // отбор контрольных образцов в рамках процесса контроля качества
     ToVerifyCompliance = 3 // отбор образцов для подтверждения соответствия в рамах технического регулирования
  };

enum ActionIDTypeEnum
  {
     RegisterControlSamples = 312,  // отбор контрольных образцов в рамках процесса контроля качества
     RegisterProductEmission = 313 // отбор образцов для подтверждения соответствия в рамах технического регулирования
  };

#endif // BASETYPES_H

