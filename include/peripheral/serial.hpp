#ifndef _PERIPHERAL_SERIAL_H_
#define _PERIPHERAL_SERIAL_H_

#include "hal/uart_hal.h"

class Serial
{
public:
  Serial(PinName RX, PinName TX, uint32_t baud_rate);
  void print(const char *str);

private:
  UART_Config *uart_config;
};

#endif // _PERIPHERAL_SERIAL_H_