#include <sam.h>

uint32_t SystemCoreClock = 8000000UL;
volatile uint32_t millis = 0;
volatile uint32_t blinkSpeed = 10;

void gclk_sync()
{
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;
}

/**
 * Set up the main clock to 8 MHz
 */
void initClocks()
{
  /*
    Plan of attack:

    1. Enable external system clock 8MHz w/o prescaler
    2. Connect Generic clock to system clock
    3. Enable generic clock
    4. 
  */
  NVMCTRL->CTRLB.bit.RWS = 1;

  // Enable external 8MHz crystal
  SYSCTRL->XOSC.reg = SYSCTRL_XOSC_STARTUP(6) | SYSCTRL_XOSC_XTALEN;
  SYSCTRL->XOSC.bit.ENABLE = 1;

  // Wait for osc to ready
  while (!SYSCTRL->PCLKSR.bit.XOSCRDY)
    ;

  // Set generator divider
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(0) | GCLK_GENDIV_DIV(0);
  gclk_sync();

  // Set GLCK_Main to xosc
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(0) | GCLK_GENCTRL_SRC_XOSC | GCLK_GENCTRL_GENEN;
  gclk_sync();

  // Disable internal oscillators
  SYSCTRL->OSC8M.bit.ENABLE = 0;

  // Configure SysTick to trigger every millisecond using the CPU Clock
  SysTick->CTRL = 0;                            // Disable SysTick
  SysTick->LOAD = SystemCoreClock / 1000 - 1UL; // Set reload register for 1mS interrupts
  NVIC_SetPriority(SysTick_IRQn, 3);            // Set interrupt priority to least urgency
  SysTick->VAL = 0;                             // Reset the SysTick counter value
  SysTick->CTRL = 0x00000007;                   // Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
  NVIC_EnableIRQ(SysTick_IRQn);                 // Enable SysTick Interrupt
}

void initSerial()
{
  blinkSpeed = 2;
  // Set Sercom0 clock from generator 0x0 (gclk_main) which is 8MHz
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM0_CORE | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;

  // Enable SERCOM0 clock
  PM->APBCSEL.bit.APBCDIV = 0;
  PM->APBCMASK.bit.SERCOM0_ = 1;
  PM->APBBMASK.bit.DMAC_ = 1;

  // Configure RX/TX pins
  PORT->Group[0].DIRSET.reg = PORT_PA10;  // PA10 = TX is output
  PORT->Group[0].PINCFG[11].bit.INEN = 1; // PA11 = RX is input

  PORT->Group[0].PINCFG[10].bit.PMUXEN = 1; // Connect PA10 to peripheral
  PORT->Group[0].PINCFG[11].bit.PMUXEN = 1; // Connect PA11 to peripheral

  PORT->Group[0].PMUX[5].reg |= PORT_PMUX_PMUXE_C | PORT_PMUX_PMUXO_C; // PMUX PA10 and PA11 to C (SERCOM)

  // Configure SERCOM0 to UART
  SERCOM0->USART.CTRLA.bit.ENABLE = 0;  // Disable
  SERCOM0->USART.CTRLA.bit.MODE = 0x1;  // Use internal clock
  SERCOM0->USART.CTRLA.bit.CMODE = 0x0; // Set comm mode to async
  SERCOM0->USART.CTRLA.bit.RXPO = 0x2;  // Use pad 2 for RX
  SERCOM0->USART.CTRLA.bit.TXPO = 0x3;  // Use pad 3 for TX
  SERCOM0->USART.CTRLA.bit.DORD = 0x0;  // MSB or LSB

  SERCOM0->USART.CTRLB.bit.CHSIZE = 0x0;
  SERCOM0->USART.CTRLB.bit.PMODE = 0x0;  // No parity mode
  SERCOM0->USART.CTRLB.bit.SBMODE = 0x1; // Single stop bit

  SERCOM0->USART.INTENSET.bit.RXC = 0x1;

  SERCOM0->USART.BAUD.bit.BAUD = SystemCoreClock / 9600;

  SERCOM0->USART.CTRLB.bit.RXEN = 0x1;
  // SERCOM0->USART.CTRLB.bit.TXEN = 0x1;

  SERCOM0->USART.CTRLA.bit.ENABLE = 1; // Enable

  // Wait for USART to become available
  while (SERCOM0->USART.SYNCBUSY.bit.ENABLE || SERCOM0->USART.SYNCBUSY.bit.CTRLB)
    ;

  NVIC_EnableIRQ(SERCOM0_IRQn);
}

int main()
{
  // Assign the LED0 pin as OUTPUT
  PORT->Group[0].DIRSET.reg = PORT_PA06;
  PORT->Group[0].OUTSET.reg = PORT_PA06;

  PORT->Group[0].DIRSET.reg = PORT_PA20;
  PORT->Group[0].OUTCLR.reg = PORT_PA20;

  // Initialize clocks
  initClocks();

  initSerial();

  blinkSpeed = 1;
  while (1)
  {
  }
}

void SERCOM0_Handler(void)
{
  // Toggle LED when UART receive completed
  if (SERCOM0->USART.INTFLAG.bit.RXC)
  {
    // PORT->Group[0].OUTTGL.reg |= PORT_PA06;
  }
}

void HardFault_Handler(void)
{
  PORT->Group[0].OUTCLR.reg |= PORT_PA06;
}

void SysTick_Handler()
{
  millis++;
  if (millis % (1000 / blinkSpeed) == 0)
  {
    PORT->Group[0].OUTTGL.reg |= PORT_PA06;
  }
}