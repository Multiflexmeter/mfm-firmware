#include "main.hpp"

void delay(uint32_t timeMS)
{
  uint32_t till = millis() + timeMS;
  while (millis() < till)
    ;
}

int main(void)
{
  init_system_clock();
  Serial serial(UART_RX, UART_TX, 9600);

  pinMode(LED, OUTPUT);
  while (1)
  {
    pinWrite(LED, HIGH);
    serial.print("LED On! :) \r\n");
    delay(1000);
    pinWrite(LED, LOW);
    serial.print("LED Off! :( \r\n");
    delay(1000);
  }
}