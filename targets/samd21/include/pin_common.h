#ifndef _PIN_COMMON_H_
#define _PIN_COMMON_H_

#define PIN_Pos 0
#define PIN_Msk 0b11111 << PIN_Pos
#define PORT_Pos 5
#define PORT_Msk 0b111 << PORT_Pos

#define PININDEX(PINNAME) (((unsigned char)PINNAME & PIN_Msk) >> PIN_Pos)
#define PINPORT(PINNAME) (((unsigned char)PINNAME & PORT_Msk) >> PORT_Pos)
#define PINNAME(PORT, INDEX) ((unsigned char)((PORT << PORT_Pos) | (INDEX << PIN_Pos)))

typedef enum
{
  NC = 0xFF,
  A00 = PINNAME(0, 0),
  A01,
  A02,
  A03,
  A04,
  A05,
  A06,
  A07,
  A08,
  A09,
  A10,
  A11,
  A12,
  A13,
  A14,
  A15,
  A16,
  A17,
  A18,
  A19,
  A20,
  A21,
  A22,
  A23,
  A24,
  A25,
  A27 = A25 + 2,
  A28,
  A30 = A28 + 2,
  A31,
  B01 = PINNAME(1, 0),
  B02,
  B03,
  B04,
  B05,
  B06,
  B07,
  B08,
  B09,
  B10,
  B11,
  B12,
  B13,
  B14,
  B15,
  B16,
  B17,
  B22 = B17 + 5,
  B23,
  B30 = B23 + 7,
  B31,
} PinName;

#endif // _PIN_COMMON_H_