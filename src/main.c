#include "main.h"

void delay()
{
  for (int i = 0; i < 100000; i++)
  {
    asm("nop");
  }
}

int main(void)
{
  pinMode(LED, OUTPUT);
  while (1)
  {
    pinWrite(LED, HIGH);
    delay();
    pinWrite(LED, LOW);
    delay();
  }
}