#include "main.h"
#include "hal/gpio.h"

void delay()
{
  for (int i = 0; i < 100000; i++)
  {
    asm("nop");
  }
}

int main(void)
{
  pinDir(0, 6, OUTPUT);
  while (1)
  {
    pinWrite(0, 6, HIGH);
    delay();
    pinWrite(0, 6, LOW);
    delay();
  }
}