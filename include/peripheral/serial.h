#ifndef _PERIPHERAL_SERIAL_H_
#define _PERIPHERAL_SERIAL_H_

#include "hal/uart_hal.h"

class Serial
{
public:
  Serial(UART_Config *config);
  void begin();
  void print(const char *str);
  char read();
  void write(char data);
  void readUntil(const char term, char *buffer, uint8_t length);

private:
  UART_Config *uart_config;
};

#endif // _PERIPHERAL_SERIAL_H_