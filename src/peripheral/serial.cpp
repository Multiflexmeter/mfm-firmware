#include "peripheral/serial.h"
#include "hal/gpio_hal.h"

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

void Serial::readUntil(const char terminator, char *buffer_ptr, uint8_t length)
{
  uint8_t index = 0;
  while (index < length)
  {
    uint8_t c = uart_getc(uart_config);
    *buffer_ptr++ = (char)c;
    if (c == terminator)
      break;
    index++;
  }
}

void Serial::write(char data)
{
  uart_putc(uart_config, data);
}

char Serial::read()
{
  uart_getc(uart_config);
}