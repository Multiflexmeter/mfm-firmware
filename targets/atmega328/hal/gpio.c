#include <avr/io.h>
#include "hal/gpio.h"

void pinDir(int group, int port, PinDirection direction)
{
  if (direction == OUTPUT)
  {
    _SFR_IO8(0x01 + 0x03 * group) |= 1 << port;
  }
  else
  {
    _SFR_IO8(0x01 + 0x03 * group) &= ~(1 << port);
  }
}

void pinWrite(int group, int port, PinLevel level)
{
  if (level == HIGH)
  {
    _SFR_IO8(0x02 + 0x03 * group) |= 1 << port;
  }
  else
  {
    _SFR_IO8(0x02 + 0x03 * group) &= ~(1 << port);
  }
}