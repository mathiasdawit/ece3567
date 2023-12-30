/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * MODIFIED BY -  Gregg Chapman, The Ohio State University, February 2018
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
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * TempSensorMode.c
 *
 * Simple thermometer application that uses the internal temperature sensor to
 * measure and display die temperature on the segmented LCD screen
 *
 * February 2015
 * E. Chen
 *
 ******************************************************************************/

#include <driverlib.h>
#include "ECE3567.h"
#include "stdio.h"
#include "string.h"

// All globals have been moved to 3567.h  NO EXTERNS NECESSARY

/******************************* PWM Output to RC Filter *******************************************/

void Init_RC()
{
    P2DIR |= 0x02;              // Set P2.1 to output
    P2SEL1 |= 0x02;             // Set P2.1 to SECONDARY OUTPUT, TB0.5, (10)
    P2SEL0 &= 0xFD;             // Set P2.1 to SECONDARY OUTPUT, TB0.5, (10)
    TB0CCTL5 = 0x00E0;          // CCR5 reset/set, ACLK
    TB0CCR5 = 0x0000;           // CCR5 PWM duty cycle gets ramped.  Zero is OK
    return;
}

/******************************* Calculate the RC Voltage from the Measured ADC Value *********************/
void RC_Voltage()
{
            // Calculate voltage from A5 ADC data
            unsigned int tempRC = (ADCValue1);
            tempRC = ((((unsigned long)tempRC)*4/5) + 28);

            // Averaging Routine
            k2++;
            sum2 = sum2 + tempRC;
            if(k2 >= Vsize)         // Vsize is the number of values to average(8 in 3567.h)
            {
                volts = sum2/(Vsize);
                sum2 = 0;
                k2 = 0;
                // Update status
                displayVolts();	// Update LCD
                RC_Feedback();	// Call feedback algorithm
            }
            return;
}

/******************************* Unpack voltage digits and display on LCD *******************************/

void displayVolts()
{
    clearLCD();
    showChar('V',pos6);                         //  V is on right

    // Handles displaying up to 9.99 Volts
    if (volts>=1000)
        showChar((volts/1000)%10 + '0',pos3);
    if (volts>=100)
        showChar((volts/100)%10 + '0',pos4);
    if (volts>=10)
        showChar((volts/10)%10 + '0',pos5);


    LCDMEM[pos3+1] |= 0x01;                    // Decimal point
    return;

}

/************************************  Used in Lab 8 to Regulate RC Voltage in Software  **********************************/

void RC_Feedback()
{
    if(volts <= RC_SetPoint + RC_high_limit && volts >= RC_SetPoint - RC_low_limit) // volts within range
    {
        __no_operation();  //do nothing (no-op)
    }
    else if (volts > RC_SetPoint) // volts larger than set point
    {
        TB0CCR5 -= step;   // subtract current step size from PWM duty cycle
    }
    else // volts smaller than set point
    {
        TB0CCR5 += step;  // add current step size to PWM duty cycle
    }
    if(step > 1){
        step = step/2;
    }
    if(volts <= RC_SetPoint + LED_high && volts >= RC_SetPoint - LED_low)
    {
        LED_Color = Green;  // set RGB LED to green using LED_Color
    }
    else
    {
        LED_Color = Red;
    }
    update_RGB();
	return;
}

/************************ Ramp the RC voltage by incrementing the TB0CCR5 Comparator Directly Lab 6  *******************/
void update_RC()		// Not called this lab
{
        TB0CCR5 +=8;
        if(TB0CCR5  > 0x0147)
        {
            TB0CCR5  = 0x0000;
        }
    return;
}
/************************************************** END OF CODE *************************************************/
