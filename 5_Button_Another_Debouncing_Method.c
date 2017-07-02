/*
This code uses the button connected to P1.3 to toggle the LED connected to P1.0 on and off.

This code implements a better method of debouncing without using the timer module.
Here, every button press is processed once and only once, no matter what the duration of press is.
However, the loop execution is stalled as long as the switch is pressed. This can be avoided by using timers and interrupts

The idea is that once we detect a switch press, we toggle the LED and then keep waiting until we detect that the switch has
not been pressed continuously for a specified number of cycles. Once we know that, we realise that the switch is now definitely not pressed
and the next time we detect a switch press, we can safely toggle the LED.

Make sure the jumper connecting the LED to pin 1.0 is connected (It is already connected by default)

 */

#include <msp430.h> 

/*
 * main.c
 */
int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR&=~BIT3; //Set P1.3 as input
    P1REN|=BIT3; //Enable pullup/pulldown resistors for P1.3
    P1OUT|=BIT3; //Set P1.3 to have pull up resistors

    P1DIR|=BIT0; //Set P1.0 as output
    P1OUT&=~BIT0; //Initially turn off the LED

    int i; //Declare counter variable i

    while(1) //Run the code forever
    {
        if(!(P1IN&BIT3)) //If button is pressed
        {

         P1OUT^=1<<0; //Toggle the LED
            i=0; //Start a counter variable
            while(i<500) //Wait till the switch is not pressed continusouly for 500 loop cycles
            {
                if((P1IN&1<<3))
                    i++; //If the switch is not pressed, increment the counter variable
                else
                    i=0; //If the switch is pressed, reset the counter variable
            }
        } //Come out of the loop as switch is now confirmed to be not pressed.

    }
    return 0;
}
