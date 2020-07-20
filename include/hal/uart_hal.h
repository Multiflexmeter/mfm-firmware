#ifndef _UART_HAL_H_
#define _UART_HAL_H_

#include <stdint.h>
#include "uart_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  void uart_init(UART_Config *config);
  uint8_t uart_getc(UART_Config *config);
  void uart_putc(UART_Config *config, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif // _UART_HAL_H_