/*
This code toggles between the red and green LEDs connected to P1.0 and P1.6 respectively on the Launchpad
"Hello, LEDs!"

Make sure both the LEDs are connected to the port pins via the jumpers. (They are already connected by default)

*/
#include <msp430.h> 

//Constants Definitions
//The value of DELAY will be proportional to the time the LED will be held on or off
#define DELAY 20000

int main(void)
{
    volatile int i; //Force the compliler to not optimize the variable 'i'
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    P1DIR|=BIT0|BIT6; //Set P1.0 and P1.6 as output pin
    while(1) //Keep toggling the LEDs forever
    {
        P1OUT|=BIT0; //Turn on red LED
        P1OUT&=~BIT6; //Turn off green LED
        for(i=DELAY;i>0;i--); //Delay
        P1OUT|=BIT6; //Turn on green LED
        P1OUT&=~BIT0; //Turn off red LED
        for(i=DELAY;i>0;i--); //Delay
    }

    return 0;
}
