/*
 * This code will fade in and fade out the LED connected on P1.0
 *
 * This code simulates a software PWM. The variable k is equivalent to the period of the PWM wave and the variable j
 * is equivalent to the duty cycle of the PWM wave (j/k, to be more precise)
 * The variable i is the loop variable. i goes from 0 to k and back to 0.
 * When i is less than j, the LED is on and when i goes from j to k, the LED is off
 * j is restricted to be always less than k
 *
 * In this setup, we vary the variable j to change the intensity of the LED
 * To get this fading or "breathing" effect, j is varied slowly from 0 to k and back to 0.
 * We use the variable l to change j
 * This makes the LED go from dim to bright and back to dim, creating the required effect
 *
 */
#include <msp430.h> 

int main(void) {
    int i,j,k,l; //Declare the needed variables
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    P1DIR|=BIT0; //Set P1.0 as output
    j=10;k=1000;l=8; //Initialise the variables
    while(1) //Run the code forever
    {
        P1OUT|=BIT0; //Turn on the LED
        for(i=0;i<j;i++); //Let the LED be on for j number of cycles
        P1OUT&=~BIT0; //Turn off the LED
        for(i=j;i<k;i++); //Let the LED be off for k-j number of cycles
        j+=l; //Change j
        if(j>990){l*=-1;} //If the LED is reaching maximum brightness, start decreaing j
        if(j<10){l*=-1;} //If the LED is reaching minimum brightness, start increasing j
    }

        return 0;
}
