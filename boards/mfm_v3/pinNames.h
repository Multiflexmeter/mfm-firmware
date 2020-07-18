#ifndef _PINNAMES_H_
#define _PINNAMES_H_

#define PIN_Pos 0
#define PIN_Msk 0x7 << PIN_Pos
#define PORT_Pos 3
#define PORT_Msk 0xF << PORT_Pos

#define PININDEX(PINNAME) (((unsigned char)PINNAME & PIN_Msk) >> PIN_Pos)
#define PINPORT(PINNAME) (((unsigned char)PINNAME & PORT_Msk) >> PORT_Pos)
#define PINNAME(PORT, INDEX) ((unsigned char)((PORT << PORT_Pos) | (INDEX << PIN_Pos)))

typedef enum
{
  D0 = PINNAME(3, 0),
  D1,
  D2,
  D3,
  D4,
  D5,
  D6,
  D7,
  D8 = PINNAME(1, 0),
  D9,
  D10,
  D11,
  D12,
  D13,
  D14 = PINNAME(2, 0),
  D15,
  D16,
  D17,
  D18,
  D19,
  D20 = PINNAME(1, 6),
  D21,
  D22 = PINNAME(2, 6)
} PinName;

#endif // _PINNAMES_H_