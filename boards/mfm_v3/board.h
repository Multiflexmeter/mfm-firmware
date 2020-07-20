#ifndef _board_h_
#define _board_h_

#include "avr/io.h"
#include "pin_names.h"
#include "peripheral/serial.h"

// Used for board configuration

#define LED B5

UART_Config EDBG_Config = {
    .sercom_index = 5,
    .baud_rate = 9600};

#endif