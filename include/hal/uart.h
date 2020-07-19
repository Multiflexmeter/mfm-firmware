#ifndef _UART_H_
#define _UART_H_

#include "pinNames.h"

#ifdef __cplusplus
extern "C"
{
#endif

  void serial_init(PinName TX, PinName RX, unsigned long baud_rate);
  int serial_getc();
  void serial_putc(char data);

#ifdef __cplusplus
}
#endif

#endif // _UART_H_