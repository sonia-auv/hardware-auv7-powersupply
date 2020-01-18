/***
 * 
 * Example code: This code is a simple program that turn on/off a LED with a button while another LED flash.
 * 
 ***/

#include "mbed.h"
#include "rtos.h"
 
InterruptIn button(PC_13);
DigitalOut led(LED2);
DigitalOut flash(LED3);
Thread thread;
 
void interrupt() {
    thread.flags_set(0x1);
}

void threadFunction()
{
  while(1)
  {
    ThisThread::flags_wait_any(0x1);
    led = !led;
  }
}
 
int main() {
    thread.start(threadFunction);
    thread.set_priority(osPriorityLow);

    button.rise(&interrupt);  // attach the address of the flip function to the rising edge
    while(1) {           // wait around, interrupts will interrupt this!
        flash = !flash;
        ThisThread::sleep_for(250);
    }
}