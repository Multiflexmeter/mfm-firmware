#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include "pinNames.h"

#ifdef __cplusplus
extern "C"
{
#endif

  void serial_init(PinName TX, PinName RX, uint32_t baud_rate);
  uint8_t serial_getc();
  void serial_putc(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif // _UART_H_