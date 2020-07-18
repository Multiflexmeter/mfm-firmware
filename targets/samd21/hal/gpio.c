#include "samd21.h"
#include "hal/gpio.h"

void pinDir(int group, int port, PinDirection direction)
{
  if (direction == OUTPUT)
  {
    PORT->Group[group].DIRSET.reg |= 1 << port;
  }
  else
  {
    PORT->Group[group].DIRCLR.reg |= 1 << port;
  }
}

void pinWrite(int group, int port, PinLevel level)
{
  if (level == HIGH)
  {
    PORT->Group[group].OUTSET.reg |= 1 << port;
  }
  else
  {
    PORT->Group[group].OUTCLR.reg |= 1 << port;
  }
}