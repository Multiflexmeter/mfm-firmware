#include "hal/clock.h"
#include "avr/power.h"
#include "avr/interrupt.h"

void init_system_clock()
{
  // TODO: Can clock be configured runtime?

  // Configure timer/counter
  PRR &= ~(1 << PRTIM1); // Enable TIMER/COUNTER 0
  OCR1B = 1000ul;
  TIMSK1 |= 1 << OCF1B;
  TCCR1B = 1 << CS11; // Set clock source, thus enabling the timer
}

unsigned long sys_millis = 0;
ISR(TIMER1_COMPB_vect)
{
  sys_millis++;
}

unsigned long millis()
{
  return sys_millis;
}
