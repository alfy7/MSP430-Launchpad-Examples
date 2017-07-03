/*
 *  This is a fun game. The red and green LEDs keep blinking alternatively. The objective of the game is to press the switch
 *  only when the green LED is on. By doing so you go to the next level, which means the blinking speed becomes faster. If you press
 *  the switch when the red LED is on, you have to start over.
 *
 *
 */
#include <msp430.h> 

int main(void) {

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR&=~BIT3; //Set P1.3 as input
    P1REN|=BIT3; //Enable pullup/pulldown resistors for P1.3
    P1OUT|=BIT3; //Set P1.3 to have pull up resistors

    P1DIR|=BIT0|BIT6; //Set P1.0 and P1.6 as output

    volatile int i,j,delay,won=2,dec=5000; //Declare necessary variables
    while(1) //Run the code forever
    {
        for(j=0;j<5;j++) //Initially, let both the LEDs blink 5 times
        {
            P1OUT|=BIT0|BIT6;
            for(i=0;i<30000;i++);
            P1OUT&=~(BIT0|BIT6);
            for(i=0;i<30000;i++);
        }
        for(i=0;i<30000;i++); //A small delay before the game starts

        dec=5000; //Initialize dec before each game

        for(delay=30000;delay>500;delay-=dec)   //The variable delay is the number of loop cycles between each toggle of the LED
            //It is starts from 30000 and is decremented after every successful switch press
        {
            won=2; //Initialize won=2, where the user has not pressed the switch yet
            while(won==2) //As long as the user has not pressed anything, keep looping
            {
                P1OUT|=BIT0; //Turn on the Red LED
                P1OUT&=~(BIT6);// Turn off the Green LED
                for(i=0;i<delay;i++) //Wait for delay number of loops
                {
                    if(!(P1IN&BIT3)){won=0;break;} //If switch is pressed now, the user has lost, make won=0 and break from this inner loop

                }
                if(won==0) break; //If lost already, break from this outer loop as well
                P1OUT|=(BIT6); //Turn on the green LED
                P1OUT&=~(BIT0); //Turn off the red LED
                for(i=0;i<delay;i++) //Wait for delay number of loops
                {
                    if(!(P1IN&BIT3)){won=1;break;} //If switch is pressed now, the user has won this round, make won=1 and break from this inner loop
                }
            }
            if(won==0) break; //If user has lost, break out of this bigger outer loop as well
            //else, the user has won this round
            //Blink the green LED twice to let the user know that the switch was pressed correctly
            for(j=0;j<2;j++)
            {
                P1OUT|=(BIT6);
                for(i=0;i<delay;i++);
                P1OUT&=~(BIT6);
                for(i=0;i<delay;i++);
            }
            //As the user progresses further in the game, decrement the rate at which delay is changed after every round
            //This adds more rounds and difficulty
            if(delay==10000) dec=1000;
            if(delay==1000) dec=100;
            //Go to the next round
        }

        if(won==0) //If user has lost, blink the red LED 2 times
        {
            for(j=0;j<2;j++)
            {
                P1OUT|=(BIT0);
                for(i=0;i<30000;i++);
                P1OUT&=~(BIT0);
                for(i=0;i<30000;i++);
            }

        }

    }
    return 0;
}
