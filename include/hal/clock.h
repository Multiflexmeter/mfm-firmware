#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>

uint32_t SystemCoreClock;

#ifdef __cplusplus
extern "C"
{
#endif

  void init_system_clock();
  uint32_t millis();

#ifdef __cplusplus
}
#endif
#endif // _CLOCK_H_