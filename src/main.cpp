#include "main.h"

void delay(uint32_t timeMS)
{
  uint32_t till = millis() + timeMS;
  while (millis() < till)
    ;
}

int main(void)
{
  init_system_clock();

  pinMode(LED, OUTPUT);

  Serial EDBG(&EDBG_Config);
  EDBG.begin();

  // Loop forever
  while (1)
  {
    pinWrite(LED, HIGH);
    EDBG.print("LED On! :) \r\n");
    delay(1000);
    pinWrite(LED, LOW);
    EDBG.print("LED Off! :( \r\n");
    delay(1000);
  }
}