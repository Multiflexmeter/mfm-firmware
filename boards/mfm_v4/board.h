#ifndef _board_h_
#define _board_h_

#include "sam.h"
#include "pin_names.h"

// Used for board configuration
#define LED A06

UART_Config EDBG_Config = {
    .RX = B23,
    .RX_pad = 3,
    .TX = B22,
    .TX_pad = 1,
    .sercom_index = 5,
    .peripheral = 3,
    .baud_rate = 9600};

#endif