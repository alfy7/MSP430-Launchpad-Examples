/*
This code uses the button connected to P1.3 to toggle the LED connected to P1.0 on and off.
Debouncing is required as small glitches can cause multiple toggles even if the switch is pressed once.

One simple and crude way of debouncing is to add a delay after a switch press is detected.
A larger delay would result in quick successive presses not processed and a smaller delay would result
in a single press processed multiple times.

Another drawback of this method is that pressing the switch continuously would result in the LED toggling on
and off continuously, which depending on the project, can be either required or not required.

Make sure the jumper connecting the LED to pin 1.0 is connected properly (It is already connected by default)
 */
#include <msp430.h> 

//Constants Definitions
//The value of DELAY will decide how sensitive the switch will be as described above
#define DELAY 150000

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    P1DIR&=~BIT3; //Set P1.3 as input
    P1REN|=BIT3; //Enable pullup/pulldown resistors for P1.3
    P1OUT|=BIT3; //Set P1.3 to have pull up resistors

    P1DIR|=BIT0; //Set P1.0 as output
    P1OUT&=~BIT0; //Initially turn off the LED

    while(1)
    {
        if((P1IN&BIT3)==0) //If the switch is pressed
        {

         P1OUT^=BIT0; //Toggle the LED
          __delay_cycles(DELAY); //Wait for switch to be unpressed
        }

    }
    return 0;
}
