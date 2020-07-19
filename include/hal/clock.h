#ifndef _CLOCK_H_
#define _CLOCK_H_

#ifdef __cplusplus
extern "C"
{
#endif

  unsigned long SystemCoreClock;

  void init_system_clock(void);

#ifdef __cplusplus
}
#endif
#endif // _CLOCK_H_