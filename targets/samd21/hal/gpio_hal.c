#include "samd21.h"
#include "pin_names.h"
#include "hal/gpio_hal.h"

void pinMode(PinName pin, PinDirection direction)
{
  if (pin == NC)
    return;

  uint8_t port = PINPORT(pin);
  uint8_t index = PININDEX(pin);
  if (direction == OUTPUT)
  {
    PORT->Group[port].DIRSET.reg |= 1 << index;
  }
  else
  {
    PORT->Group[port].DIRCLR.reg |= 1 << index;
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
    PORT->Group[port].OUTSET.reg |= 1 << index;
  }
  else
  {
    PORT->Group[port].OUTCLR.reg |= 1 << index;
  }
}