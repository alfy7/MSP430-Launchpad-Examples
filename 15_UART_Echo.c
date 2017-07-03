/*
 *  This code is taken from the MSP430Ware code examples
 *  It illustrates basic UART functionality. We send a character from the PC
 *  to the MSP430 and the MSP430 replies back with the same character
 *  You would need a serial console interface like Putty to communicate on the PC
 *
 *  The default settings 9600, 8 data bits, no parity and 1 stop bit are used in this example
 *
 *  If you are using the Rev.1.5 version of the Launchpad, you need to switch the shorting jumpers of the Tx and Rx horizontally
 *  (Looking at the launchpad with the USB port on the top)
 *  This is NOT done by default. The Vcc, Test and Reset will be vertical and the Tx/Rx shorting jumpers will be horizontal after you make the change.
 *  For other versions, you would need extra jumper wires. Do verify.
 *
 *  This code may not work on all newer systems. It could be a driver issue or USB3.0 issue or protocol issue
 *  or a multiple of them. Basically, the programmer on the Launchpad does not communicate properly with newer PCs (as far as I can tell)
 *  For me, it works rarely, but it is unpredictable. However, using the MSP430 on a separate board with a
 *  UART-USB converter (like the CH340) always works.
 *
 *  If it doesn't work for you, you would need a way to bypass the programmer to make it work.
 */


#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop Watchdog timer

  //We need the clock speed to be accurate for UART communications
  //Using the DCO, we get approximately the frequency as calculated in the datasheet
  //However, we need more accuracy to be able to communicate with minimal error
  //All MSP430s have been calibrated to a set of standard frequencies and this calibration data is stored
  //in the MSP430 itself. This data is mostly unique to every device and can be used to get accurate clock frequency
  //when we do not have a crystal oscillator
  //However, there are ways in which we might erase this calibration data, so we check for it before proceeding
  if (CALBC1_1MHZ==0xFF)                    // If calibration constant is erased
  {
    while(1);                               // Do not execute
  }

  //Set clock to 1Mhz
  DCOCTL = 0;
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;

  //Setup UART Communications 9600,8,N,1
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM0_bits + GIE);       // Enter Low Power Mode 0, interrupts enabled
}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(IFG2&UCA0TXIFG));                // Wait until USCI_A0 TX buffer is ready
  UCA0TXBUF = UCA0RXBUF;                    // Transmit the received character
}


/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
