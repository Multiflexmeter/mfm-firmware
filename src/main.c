#include "main.h"

void delay(unsigned long timeMS)
{
  unsigned long till = millis() + timeMS;
  while (millis() < till)
    ;
}

int main(void)
{
  init_system_clock();

  pinMode(LED, OUTPUT);
  while (1)
  {
    pinWrite(LED, HIGH);
    delay(1000);
    pinWrite(LED, LOW);
    delay(1000);
  }
}