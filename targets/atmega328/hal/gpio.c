#include <avr/io.h>
#include <pinNames.h>
#include "hal/gpio.h"

void pinMode(PinName pin, PinDirection direction)
{
  if (pin == NC)
    return;

  uint8_t port = PINPORT(pin);
  uint8_t index = PININDEX(pin);

  if (direction == OUTPUT)
  {
    _SFR_IO8(0x01 + 0x03 * port) |= 1 << index;
  }
  else
  {
    _SFR_IO8(0x01 + 0x03 * port) &= ~(1 << index);
  }
}

void pinWrite(PinName pin, PinLevel level)
{
  if (pin == NC)
    return;

  uint8_t port = PINPORT(pin);
  uint8_t index = PININDEX(pin);

  if (level == HIGH)
  {
    _SFR_IO8(0x02 + 0x03 * port) |= 1 << index;
  }
  else
  {
    _SFR_IO8(0x02 + 0x03 * port) &= ~(1 << index);
  }
}