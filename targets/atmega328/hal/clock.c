#include "hal/clock.h"
#include "avr/io.h"

void init_system_clock()
{
  // TODO: Can clock be configured runtime?
  SystemCoreClock = 8000000ul;

  // Configure timer/counter
  PRR &= ~(1 << PRTIM1);              // Enable TIMER/COUNTER 0
  OCR1B = SystemCoreClock / 8 / 1000; // 8MHz / 8 = 1MHz / 1000 = 1000Hz, thus every 1 ms
  TIMSK1 |= 1 << OCF1B;               // Set timer compare interrupt flag
  TCCR1B = 1 << CS11;                 // Set clock src/divider, thus enabling timer
}

uint32_t sys_millis = 0;
ISR(TIMER1_COMPB_vect)
{
  sys_millis++;
}

uint32_t millis()
{
  return sys_millis;
}
