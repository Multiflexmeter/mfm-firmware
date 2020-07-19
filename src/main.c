#include "main.h"

void delay()
{
  for (int i = 0; i < 100000; i++)
  {
    asm("nop");
  }
}

unsigned long lastTick = 0;
int main(void)
{
  init_system_clock();

  pinMode(LED, OUTPUT);
  char state = 0;
  while (1)
  {
    if (millis() - lastTick >= 1000)
    {
      state = !state;
      pinWrite(LED, state);
      lastTick = millis();
    }
  }
}