#include <sam.h>

int main()
{
  PORT->Group[0].DIRSET.reg |= PORT_PA06;

  while (1)
  {
    PORT->Group[0].OUTTGL.reg |= PORT_PA06;
    for (int x = 0; x < 30000; x++)
    {
      __ASM("nop");
    }
  }
}