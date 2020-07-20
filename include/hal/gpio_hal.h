#ifndef _GPIO_HAL_H_
#define _GPIO_HAL_H_

#include "pin_names.h"

typedef enum
{
  INPUT = 0,
  OUTPUT = 1,
} PinDirection;

typedef enum
{
  LOW = 0,
  HIGH = 1
} PinLevel;

#ifdef __cplusplus
extern "C"
{
#endif

  void pinMode(PinName pin, PinDirection val);
  void pinWrite(PinName pin, PinLevel val);

#ifdef __cplusplus
}
#endif
#endif // _GPIO_HAL_H_