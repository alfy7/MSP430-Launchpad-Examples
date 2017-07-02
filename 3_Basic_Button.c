/*
This code uses the Button connected to P1.3 to control the LED connected on P1.0 (on the launchpad)

When the button is pressed, P1.3 is grounded and when it is not pressed, P1.3 is at high impedance state.
Hence, we use the internal pull up resistors available on the MSP430G2553 to treat this high impedance state as logical HIGH.
So when the button is pressed, P1.3 will be read as logical LOW and when the button is not pressed, the button will be read as logical HIGH.

When the button is pressed, this code turns on the LED and when it is not pressed, the LED is turned off.

No debouncing is needed for this code as it will not affect the state of the LED noticeably.
*/
#include <msp430.h> 

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    P1DIR&=~BIT3; //Set P1.3 as input
    P1REN|=BIT3; //Enable pullup/pulldown resistors for P1.3
    P1OUT|=BIT3; //Set P1.3 to have pull up resistors

    P1DIR|=BIT0; //Set P1.0 as output

    while(1) //Run the below code forever
    {
        if((P1IN&BIT3)==(BIT3)) //If P1.3 is logical HIGH
            P1OUT&=~BIT0; //Turn off the LED
        else
            P1OUT|=BIT0; //Else, turn on the LED
    }
    return 0;
}
