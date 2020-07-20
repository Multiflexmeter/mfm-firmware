#include "hal/uart_hal.h"
#include "hal/gpio_hal.h"
#include "pinNames.h"
#include "sam.h"

void enable_pmux(PinName pin, uint8_t peripheral)
{
  uint8_t pmux_pos = (PININDEX(pin) & 0x01u) * 4; // first bit indicated odd/even. If odd pos = 4 otherwise 0
  uint8_t port = PINPORT(pin);
  uint8_t index = PININDEX(pin);

  PORT->Group[port].PINCFG[index].bit.PMUXEN = 1;                  // Enable PMUX
  PORT->Group[port].PMUX[index / 2].reg &= ~(0xF << pmux_pos);     // Clear existing pmux peripheral
  PORT->Group[port].PMUX[index / 2].reg |= peripheral << pmux_pos; // Set pmux peripheral
}

void serial_init(PinName TX, PinName RX, uint32_t baud_rate)
{
  /**
   * Configure RX TX Pins
   */

  // Configure TX/RX IN/OUT
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);

  // Enable and set peripheral D (0x3)
  enable_pmux(TX, 0x3);
  enable_pmux(RX, 0x3);

  /**
   * Configure sercom5 clock and Peripheral power
   */

  PM->APBCMASK.reg |= PM_APBCMASK_SERCOM5;

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM5_CORE | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_WRTLOCK;

  /**
   * Enable SERCOM power
   * Attach GCLK0 to SERCOM
   * Configure SERCOM to UART
   * Enable interrupts
   */
  SERCOM5->USART.CTRLA.bit.ENABLE = 0;
  while (SERCOM5->USART.SYNCBUSY.bit.ENABLE)
    ;

  SERCOM5->USART.CTRLA.bit.SWRST = 1;
  while (SERCOM5->USART.SYNCBUSY.bit.SWRST)
    ;

  SERCOM5->USART.CTRLA.reg = SERCOM_USART_CTRLA_MODE(1) | SERCOM_USART_CTRLA_RXPO(3) | SERCOM_USART_CTRLA_TXPO(1) | SERCOM_USART_CTRLA_DORD;
  SERCOM5->USART.CTRLB.reg = SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_CHSIZE(0);
  SERCOM5->USART.BAUD.reg = (uint16_t)(0xFFFF * (1 - (16 * (baud_rate / (double)SystemCoreClock))));

  while (SERCOM5->USART.SYNCBUSY.bit.CTRLB)
    ;

  SERCOM5->USART.CTRLA.bit.ENABLE = 1;

  while (SERCOM5->USART.SYNCBUSY.bit.ENABLE)
    ;
}

uint8_t serial_getc()
{
  while (!SERCOM5->USART.INTFLAG.bit.RXC)
    ;
  return (int)SERCOM5->USART.DATA.reg;
}

void serial_putc(uint8_t data)
{
  while (!SERCOM5->USART.INTFLAG.bit.DRE)
    ;
  SERCOM5->USART.DATA.reg = data & 0xFF;
}