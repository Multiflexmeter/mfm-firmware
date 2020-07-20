#ifndef _PIN_COMMON_H_
#define _PIN_COMMON_H_

#include <stdint.h>

#define PIN_Pos 0
#define PIN_Msk 0x7 << PIN_Pos
#define PORT_Pos 3
#define PORT_Msk 0xF << PORT_Pos

#define PININDEX(PINNAME) (((uint8_t)PINNAME & PIN_Msk) >> PIN_Pos)
#define PINPORT(PINNAME) (((uint8_t)PINNAME & PORT_Msk) >> PORT_Pos)
#define PINNAME(PORT, INDEX) ((uint8_t)((PORT << PORT_Pos) | (INDEX << PIN_Pos)))

typedef enum
{
  NC = 0xFF,
  B0 = PINNAME(1, 0),
  B1,
  B2,
  B3,
  B4,
  B5,
  B6,
  B7,
  C0 = PINNAME(2, 0),
  C1,
  C2,
  C3,
  C4,
  C5,
  D0 = PINNAME(3, 0),
  D1,
  D2,
  D3,
  D4,
  D5,
  D6,
  D7
} PinName;

#endif // _PIN_COMMON_H_