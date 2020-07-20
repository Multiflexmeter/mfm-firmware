#include "hal/uart_hal.h"
#include "hal/gpio_hal.h"
#include "pin_names.h"
#include "sam.h"

Sercom *sercom_list[] = SERCOM_INSTS;

void enable_pmux(PinName pin, uint8_t peripheral)
{
  uint8_t pmux_pos = (PININDEX(pin) & 0x01u) * 4; // first bit indicated odd/even. If odd pos = 4 otherwise 0
  uint8_t port = PINPORT(pin);
  uint8_t index = PININDEX(pin);

  PORT->Group[port].PINCFG[index].bit.PMUXEN = 1;                  // Enable PMUX
  PORT->Group[port].PMUX[index / 2].reg &= ~(0xF << pmux_pos);     // Clear existing pmux peripheral
  PORT->Group[port].PMUX[index / 2].reg |= peripheral << pmux_pos; // Set pmux peripheral
}

void uart_init(UART_Config *config)
{
  uint8_t sercom_index = config->sercom_index;
  Sercom *sercom = sercom_list[sercom_index];

  /**
   * Configure RX TX Pins
   */

  // Configure TX/RX IN/OUT
  pinMode(config->TX, OUTPUT);
  pinMode(config->RX, INPUT);

  // Enable and set peripheral D (0x3)
  enable_pmux(config->TX, config->peripheral);
  enable_pmux(config->RX, config->peripheral);

  /**
   * Configure sercom5 clock and Peripheral power
   */

  PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + sercom_index);
  GCLK->CLKCTRL.reg = (GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + sercom_index) << GCLK_CLKCTRL_ID_Pos | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_WRTLOCK;

  /**
   * Enable SERCOM power
   * Attach GCLK0 to SERCOM
   * Configure SERCOM to UART
   * Enable interrupts
   */
  sercom->USART.CTRLA.bit.ENABLE = 0;
  while (sercom->USART.SYNCBUSY.bit.ENABLE)
    ;

  sercom->USART.CTRLA.bit.SWRST = 1;
  while (sercom->USART.SYNCBUSY.bit.SWRST)
    ;

  sercom->USART.CTRLA.reg = SERCOM_USART_CTRLA_MODE(1) | SERCOM_USART_CTRLA_RXPO(config->RX_pad) | SERCOM_USART_CTRLA_TXPO(config->TX_pad) | SERCOM_USART_CTRLA_DORD;
  sercom->USART.CTRLB.reg = SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_CHSIZE(0);
  sercom->USART.BAUD.reg = (uint16_t)(0xFFFF * (1 - (16 * (config->baud_rate / (double)SystemCoreClock))));

  while (sercom->USART.SYNCBUSY.bit.CTRLB)
    ;

  sercom->USART.CTRLA.bit.ENABLE = 1;

  while (sercom->USART.SYNCBUSY.bit.ENABLE)
    ;
}

uint8_t uart_getc(UART_Config *config)
{

  uint8_t sercom_index = config->sercom_index;
  Sercom *sercom = sercom_list[sercom_index];

  while (!sercom->USART.INTFLAG.bit.RXC)
    ;
  return (int)sercom->USART.DATA.reg;
}

void uart_putc(UART_Config *config, uint8_t data)
{

  uint8_t sercom_index = config->sercom_index;
  Sercom *sercom = sercom_list[sercom_index];

  while (!sercom->USART.INTFLAG.bit.DRE)
    ;
  sercom->USART.DATA.reg = data & 0xFF;
}