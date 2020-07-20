#include "peripheral/serial.h"

Serial::Serial(UART_Config *config) : uart_config(config) {}

void Serial::begin()
{
  uart_init(uart_config);
}

void Serial::print(const char *ptr)
{
  do
  {
    uart_putc(uart_config, *ptr++);
  } while (*ptr != '\0');
}