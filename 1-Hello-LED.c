/*
Dubbed as the "Hello, World!" equivalent for microcontrollers (and appropriately called "Hello, LED"),
this code will keep toggling on and off the LED connected on the launchpad to pin P1.0

Make sure the jumper connecting the LED to pin 1.0 is connected properly (It is already connected by default)
*/
#include <msp430.h> 

//Constants Definitions
//The value of DELAY will be proportional to the time the LED will be held on or off
#define DELAY 20000

int main(void) {
    volatile int i; //Force the compliler to not optimize the variable 'i'
    WDTCTL = WDTPW | WDTHOLD;	//Stop watchdog timer
    P1DIR|=BIT0; //Set P1.0 as output pin

    while(1) //Keep toggling the LED forever
    {
        P1OUT^=BIT0; //Toggle the pin state from HIGH to LOW or from LOW to HIGH
        for(i=DELAY;i>0;i--); //Simple FOR loop to create the delay
    }

	return 0;
}
