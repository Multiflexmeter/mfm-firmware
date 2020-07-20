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

  // Initialize SERCOM5 as UART
  serial_init(UART_TX, UART_RX, 9600);

  pinMode(LED, OUTPUT);
  while (1)
  {
    pinWrite(LED, HIGH);
    serial_putc('1');
    delay(1000);
    pinWrite(LED, LOW);
    serial_putc('0');
    delay(1000);
  }
}