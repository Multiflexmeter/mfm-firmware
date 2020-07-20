#ifndef _UART_COMMON_H_
#define _UART_COMMON_H_

#include <stdint.h>

typedef struct
{
  uint8_t sercom_index;
  uint16_t baud_rate;
} UART_Config;

#endif // _UART_COMMON_H_