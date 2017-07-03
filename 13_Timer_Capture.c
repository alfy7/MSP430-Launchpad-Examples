/*
 * Now that we can detect a button press accurately, we try to find the time between two presses and execute
 * different actions based on that.
 *
 * We use the Timer1 module to keep track of time and the capture module to get the exact time the button is pressed
 * We use Timer0 and Port1 interrupts to debounce the switches. We run Timer1 and use its Capture module (triggered by software)
 * to get the value of the Timer1 register, which is then converted to time.
 * We count the number of times the timer1 has overflowed in between two key presses. Using this and the capture information,
 * we get the duration of key press
 *
 * The capture module is triggered by software if we successfully detect a key press. This is done by setting the capture module to
 * The trigger on both rising and falling edge, and then everytime we detect a key press, we toggle its state between HIGH and LOW
 * (By connecting it to Vcc and Gnd respectively)
 *
 * Here is the flowchart/outline of the process
 * 00-> Start Timer1 such that it overflows every 0.01 sec (Using CCR0). Enable Port1 interrupt.
 * 01-> Detect any bounce on the switch using Port1 Interrupt
 * 02-> If bounce was detected, disable Port1 interrupt and enable Timer0 Interrupt. Keep checking the switch until it is consistently in the same state
 * 03-> Now that we have debounced the switch, disable Timer0 and enable Port1 interrupt. If switch was not pressed, go back to (01)
 * 04-> The switch was pressed. If this is the second (even number) time the switch was pressed, go to (08)
 * 05-> The switch was pressed for the first time(odd number), trigger the capture (CCR1) module of Timer1 and store the value of Timer1 now
 * 06-> Keep track of the number of times Timer1 overflows from now until the next key press
 * 07-> Go to (01) to detect next key press
 * 08-> The switch was pressed for the second time (even number), trigger the capture (CCR1) module of Timer1 and store the value of Timer1 now
 * 09-> Using the value of number of overflows from the last key press until now, the value of Timer1 at the first keypress and the value of Timer1
 *     now (at the second key press), find the time between both the key presses and execute appropriate code
 * 10-> Go to (01) to detect next key press
 */
#include <msp430.h> 

int state=0,val1=0,val2=0,count=0,time,press=0,overflow=0; //Declare necessary variables
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR&=~BIT3; //Set P1.3 as input
    P1REN|=BIT3; //Enable pullup/pulldown resistors for P1.3
    P1OUT|=BIT3; //Set P1.3 to have pull up resistors
    P1IE|=BIT3; //Enable interrupt on P1.3
    P1IES|=BIT3; //Set interrupt flag on the falling edge of logic level on P1.3


    P1DIR|=BIT0|BIT6; //Set P1.0 and P1.6 as output

    TACCR0=1000; //Make the Timer0 count from 0 to 10000, which will take  ~0.001 seconds, for debouncing

    TA1CCR0=10000-1; //Setup Timer1 to overflow approximately every 0.01 seconds, to calculate time between key presses
    TA1CCTL1|=CM_3+CCIS_2+SCS+CAP+CCIE; //Setup Timer1 with synchronous capture on both edges, connected to Vcc
                                        //and generate interrupt on capture
    TA1CTL|=TASSEL_2+MC_1+TAIE; //Setup Timer1 to run on SMCLK, UP mode and generate interrupt on overflow

    __enable_interrupt(); //Enable maskable interrupts
    while(1)
    {

        if(press==0) //If press is zero, as in, there are an even number of key presses
        {
            time=((val2-val1)/10000)+overflow; //Calculate the time between the key presses

            if(time<50) //If the time is less than 0.5 seconds
            {
                P1OUT|=BIT0;P1OUT&=~BIT6; //Turn on red LED
            }
            else if(time<100) //If time is less than 1 sec
            {
                P1OUT|=BIT6;P1OUT&=~(BIT0); //Turn on green LED
            }
            else
            {
                P1OUT|=(BIT0)|(BIT6); //Otherwise turn on both LEDs
            }

        }

    }

    return 0;
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TMR1()
{
    if(TA1IV==TA1IV_TAIFG) //Check if Timer1 overflow caused the interrupt
        //This is now required in this project as we have both Timer1 overflow interrupt
        //and Capture module interrupt generated in this project
    {
        if(press==1) overflow++; //If first key press was already detected, increment the overflow counter
        TA1CTL&=~(TAIFG); //Reset overflow flag
    }
    else //If Capture module caused the interrupt
    {
        if(press==0) //If first press was not detected
        {
            press++; //This is the first press, make press=1
            overflow=0; //Reset overflow counter to start counting overflows from now
            val1=TA1CCR1; //Save the value of Timer1 at the time of first key press
        }
        else //If this is the second key press
        {
            val2=TA1CCR1; //Save the value of Timer1 at the time of second key press
            press=0; //Reset press variable, the main function will calculate the time and take appropriate decision
        }
        TA1CCTL1&=~(CCIFG); //Reset Capture Flag
    }
}


#pragma vector=PORT1_VECTOR
__interrupt void P1_Function()
{
    count=0; //Reset count
    TACTL|=TASSEL_2+MC_1+TAIE; //Start Timer0 with SMCLK clock source, UP mode and enable overflow interrupt
    state=(P1IN&BIT3)>>3; //Save the state of the switch
    P1IE&=~BIT3; //Disable interrupt on P1.3, now the Timer will take care of Debouncing
    P1IFG&=~BIT3; // Reset Port1 interrupt flag
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TMR0()
{
    if(TAIV==TA0IV_TAIFG)   //Check if Timer0 overflow caused the interrupt
                            //This would be required in projects where multiple interrupts have
                            //the same interrupt vector. Here it is only optional.
        {
            if(state==((P1IN&BIT3)>>3)) //If the state of the LED is the same
                count++; //Increment the counter variable
            else
            {
                count=0; //If not same, reset the counter variable
                state=((P1IN&BIT3)>>3); //And save the present state of the switch
            }
        if(count==10) //If the state has been consistently the same
            {
            if(state==0) //If the switch was pressed
                TA1CCTL1^=1<<12; //Trigger the Capture module
            P1IE|=BIT3; //We have handled the debouncing, now we again enable interrupt on P1.3, for it to again detect switch bounce
            TACTL=0; //Stop the Timer
            TACTL|=TACLR; //Clear the Timer counter
            }

            TACTL&=~(TAIFG); //Reset the interrupt flag
        }
}

