#ifndef _CLOCK_H_
#define _CLOCK_H_

unsigned long SystemCoreClock;

#ifdef __cplusplus
extern "C"
{
#endif

  void init_system_clock(void);

#ifdef __cplusplus
}
#endif
#endif // _CLOCK_H_