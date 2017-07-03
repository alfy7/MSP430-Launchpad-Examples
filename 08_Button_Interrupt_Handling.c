/*
 * This code uses Port1 interrupts to detect switch press on P1.3 and toggle the LED connected on P1.0
 *
 * However, this code uses the same method as earlier to process each switch press once and only once.
 * This requires the use of a while loop (which can run for a considerable amount of time) in the interrupt service routine.
 * Even using the delay method will cause the interrupt service routine to run considerably long
 *
 * Interrupt service routine should ideally be lines of code which run quickly so that the main program can resume immediately
 * It should not contain delays or wait for an event to occur.
 *
 * This code just serves to illustrate the way that an interrupt can be used to achieve the same functionality
 * The only benefit here is that the MSP430 goes to low power mode when the switch is not pressed, reducing power consumption
 *
 * The use of a timer interrupt along with the Port1 interrupt will achieve proper debouncing (to be done soon)
 *
 */
#include <msp430.h> 

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR&=~BIT3; //Set P1.3 as input
    P1REN|=BIT3; //Enable pullup/pulldown resistors for P1.3
    P1OUT|=BIT3; //Set P1.3 to have pull up resistors

    P1IE|=BIT3; //Enable interrupt on P1.3
    P1IES|=BIT3; //Set interrupt flag on the falling edge of logic level on P1.3

    __enable_interrupt(); //Enable maskable interrupts

    P1DIR|=BIT0; //Set P1.0 as output
    P1OUT&=~BIT0; //Initially turn off the LED


    __low_power_mode_0(); //Go to low power mode 0
     return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_Function()
{
    int i; //Declare counter variable
    P1OUT^=BIT0; //Toggle the LED
    i=0; //Start a counter variable
    while(i<500) //Wait till the switch is not pressed continusouly for 500 loop cycles
    {
        if((P1IN&BIT3))
            i++;  //If the switch is not pressed, increment the counter variable
        else
            i=0; //If the switch is pressed, reset the counter variable
    }
    P1IFG&=~BIT3; // Reset interrupt flag
}
