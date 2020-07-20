#include <avr/io.h>
#include "hal/clock_hal.h"
#include "hal/uart_hal.h"

void uart_init(UART_Config *config)
{
  uint16_t baud = (uint16_t)(0xFFFF * (1 - (16 * (config->baud_rate / (double)SystemCoreClock))));

  // Set baud
  UBRR0H = baud >> 8;
  UBRR0L = baud;

  // Enable RX/TX
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  // 8bit character size, 1 stop bit, no parity
  UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

uint8_t uart_getc(UART_Config *config)
{
  while (!(UCSR0A & (1 << RXC0)))
    ;

  return UDR0 & 0xFF;
}

void uart_putc(UART_Config *config, uint8_t data)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = data;
}