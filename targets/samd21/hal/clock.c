#include "sam.h"
#include "hal/clock.h"

void wait_xosc_ready()
{
  while (!SYSCTRL->PCLKSR.bit.XOSCRDY)
    ;
}

void wait_clock_sync()
{
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;
}

/**
 * Use the external oscillator as main clock source.
 */
void init_system_clock()
{
  SYSCTRL->XOSC.reg = SYSCTRL_XOSC_XTALEN | SYSCTRL_XOSC_STARTUP(6);
  SYSCTRL->XOSC.bit.ENABLE = 1;
  wait_xosc_ready();
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(0) | GCLK_GENDIV_DIV(0);
  wait_clock_sync();
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0) | GCLK_GENCTRL_SRC_XOSC | GCLK_GENCTRL_GENEN;
  wait_clock_sync();

  // Disable the internal oscillator
  SYSCTRL->OSC8M.bit.ENABLE = 0;

  // Update main clock speed
  SystemCoreClock = 8e6;

  // Initialize systick interrupt every 1 ms
  SysTick_Config(SystemCoreClock / 1000);
}

unsigned long sys_millis = 0;
void SysTick_Handler()
{
  sys_millis++;
}

unsigned long millis()
{
  return sys_millis;
}