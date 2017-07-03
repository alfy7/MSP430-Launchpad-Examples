/*
 * This code uses the Timer module and Capture/Compare module to generate a PWM waveform which is used to control the intensity of
 * the LED connected to P1.6
 *
 * We use the LED on P1.6 instead of the one on P1.0 as the capture/compare module can directly control P1.6 (internally configured)
 * We could use the LED on P1.0, but this will require the CPU to turn on and off the LED when the timer capture/compare gives an interrupt
 *By using P1.6, the main program only needs to update the duty cycle of the PWM wave and the Timer module/Capture/Compare module will do the rest
 *
 * We configure the timer to count from 0 to TACCR0 and back to 0 in UP mode
 * When the timer counter is less than TACCR1, the LED in on and when the timer counter is between TACCR1 and TACCRO, the LED is off
 * We constrain TACCR1 to be between 0 and TACCR0
 *
 * In the main program, we change TACCR1 from 0 to TACCR0 and back to 0, giving the fading or breathing effect
*/
#include <msp430.h> 

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    //Configure P1.6 to be controlled by Capture/Compare module
    P1DIR|=BIT6;
    P1OUT&=~(BIT6);
    P1SEL|=BIT6;

    TACCR0=10000; //Setting up the PWM Frequency
    TACCR1=0; //Initialising the Duty cycle to 0
    TACCTL1|=OUTMOD_7; //Using Reset/Set mode to generate PWM wave
    TACTL|=TASSEL_2+MC_1; //Configuring the timer to use SMCLK and count in UP mode

    int pwm=0,l=100; //Initializing required variables
    while(1)
    {   __delay_cycles(5000); //Wait before changing the duty cycles, for smooth variation
        pwm+=l; //Change the duty cycle
        if(pwm==10000) l=-100;//If duty cycle is at maximum, start decreasing it
        if(pwm==0) l=100; //If duty cycle is at minimum, start increasing it
        TACCR1=pwm; //Set the duty cycles in the Capture/compare module
    }

    return 0;
}

/*
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TMR0()
{
    if(TAIV==TA0IV_TAIFG)
    { count++;
    if(count==10)
    { count =0;
    pwm+=v;
    if(pwm==10000) v=-1000;
    if(pwm==0) v=1000;
    TACCR1=pwm;
    }
    }
}
*/
