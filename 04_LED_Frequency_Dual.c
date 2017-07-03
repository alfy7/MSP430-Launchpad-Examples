/*
 * Now we try to change the frequency of both the LEDs blinking alternatively
 * This code varies the frequency of blinking of the LEDs connected to P1.0 and P1.6 using FOR loops
 *
 * The variable i is the delay loop variable. i runs from j to 0.
 * Hence the value of j decides how long the delay will be.
 * We vary j from 0 to 10000 and back to 0 in steps of 250 to achieve the desired effect
 */

#include <msp430.h> 

int main(void) {
    volatile int i,j; //Force the compiler to not optimize the variables i and j
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR|=BIT0|BIT6; //Set P1.0 and P1.6 as output
    P1OUT|=BIT0; //Initially turn on the LED on P1.0
    P1OUT&=~(BIT6); //Initially turn off the LED on P1.6
    while(1)
    {

        for(j=0;j<10000;j+=250) //j goes from 0 to 10000 in steps of 250
            {
            P1OUT^=BIT0|BIT6; //Toggle the LEDs
            for(i=j;i>0;i--); //Wait for j number of cycles

        }
        for(j=10000;j>0;j-=250) //j goes from 10000 to 0 in steps of 250
        {
            P1OUT^=BIT0|BIT6; //Toggle the LEDs
            for(i=j;i>0;i--); //Wait for j number of cycles

        }
    }

    return 0;
}
