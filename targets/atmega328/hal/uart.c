#include <avr/io.h>
#include "hal/clock.h"
#include "hal/uart.h"

void serial_init(PinName TX, PinName RX, uint32_t baud_rate)
{
  uint16_t baud = (uint16_t)(0xFFFF * (1 - (16 * (baud_rate / (double)SystemCoreClock))));

  // Set baud
  UBRR0H = baud >> 8;
  UBRR0L = baud;

  // Enable RX/TX
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  // 8bit character size, 1 stop bit, no parity
  UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

uint8_t serial_getc()
{
  while (!(UCSR0A & (1 << RXC0)))
    ;

  return UDR0 & 0xFF;
}

void serial_putc(uint8_t data)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = data;
}