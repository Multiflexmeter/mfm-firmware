#include "peripheral/serial.hpp"

Serial::Serial(PinName RX, PinName TX, uint32_t baud_rate)
{
  UART_Config config = {};
  config.RX = RX;
  config.TX = TX;
  config.baud_rate = baud_rate;
  config.char_size = 0x8;
  config.stop_bits = 0x1;

  uart_config = &config;
  uart_init(uart_config);
}

void Serial::print(const char *ptr)
{
  do
  {
    uart_putc(uart_config, *ptr++);
  } while (*ptr != '\0');
}