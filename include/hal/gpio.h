#ifndef _gpio_h_
#define _gpio_h_

typedef enum
{
  INPUT = 0,
  OUTPUT = 1,
} PinDirection;

typedef enum
{
  LOW = 0,
  HIGH = 1
} PinLevel;

#ifdef __cplusplus
extern "C"
{
#endif

  void pinDir(int group, int pin, PinDirection val);
  void pinWrite(int group, int pin, PinLevel val);

#ifdef __cplusplus
}
#endif
#endif