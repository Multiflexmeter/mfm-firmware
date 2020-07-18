#ifndef _PINNAMES_H_
#define _PINNAMES_H_

#define PIN_Pos 0
#define PIN_Msk 0b11111 << PIN_Pos
#define PORT_Pos 5
#define PORT_Msk 0b111 << PORT_Pos

#define PININDEX(PINNAME) (((unsigned char)PINNAME & PIN_Msk) >> PIN_Pos)
#define PINPORT(PINNAME) (((unsigned char)PINNAME & PORT_Msk) >> PORT_Pos)
#define PINNAME(PORT, INDEX) ((unsigned char)((PORT << PORT_Pos) | (INDEX << PIN_Pos)))

typedef enum
{
  D0 = PINNAME(0, 0),
  D1,
  D2,
  D3,
  D4,
  D5,
  D6,
  D7,
  D8,
  D9,
  D10,
  D11,
  D12,
  D13,
  D14,
  D15,
  D16,
  D17,
  D18,
  D19,
  D20,
  D21,
  D22,
  D23,
  D24,
  D25,
  D26 = D25 + 2,
  D27,
  D28 = D27 + 2,
  D29,
  D30 = PINNAME(1, 2),
  D31,
  D32 = PINNAME(1, 8),
  D33,
  D34,
  D35,
  D36 = PINNAME(1, 22),
  D37,
} PinName;

#endif // _PINNAMES_H_