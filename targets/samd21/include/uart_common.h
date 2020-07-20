#ifndef _UART_COMMON_H_
#define _UART_COMMON_H_

#include "sam.h"
#include "pin_common.h"

typedef struct
{
  PinName RX;
  uint8_t RX_pad;
  PinName TX;
  uint8_t TX_pad;
  uint8_t sercom_index;
  uint8_t peripheral;
  uint16_t baud_rate;
} UART_Config;

#endif // _UART_COMMON_H_