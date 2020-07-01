#include <mbed.h>
// #include <LowPowerTimer.h>

DigitalOut led(PA06);
// DigitalOut loraSW(PA17);
// Serial lora(PA18, PA19, 9600);
// Serial serial(PA10, PA11, 115200);
Timeout t;


void isr1() {
  led != led;
}

int main() {
  t.attach(&isr1, 5);
  while(1) {
    deepsleep();
  }
}