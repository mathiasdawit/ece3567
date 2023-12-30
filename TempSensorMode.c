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

#include "driverlib.h"
#include "ECE3567.h"
#include "TempSensorMode.h"

// All globals have been moved to 3567.h  NO EXTERNS NECESSARY

/********************************************************************/



/**************************  Reset Average and Control Booleans ******************************/

void tempSensorModeInit()
{

    k=0;            // Counters for Temperature averaging
    sum = 0;
    deg = 0;
    degF = 0;
    degC = 0;
    tempUnit = Fahrenheit;
// Initialize the heating element.  MUST BE INCLUDED to prevent unintended burn-up
      	  // 2.0 is output
        P2DIR |= BIT0;
     	 // 00 is general purpose I/O
        P2SEL1 &= ~BIT0;
        P2SEL0 &= ~BIT0;
     	 // Included as fail safe measure
      	  // Make sure the heating element if OFF
        P2OUT &= ~BIT0;
}
/************************************  Calculate Fahrenheit or Celsius Temperature from ADC Value.  Averages and Displays ********************/
void tempSensor()
{
            signed short temp = (ADCValue0 - CALADC_12V_30C);
            degC = ((long)temp * 10 * (85-30) * 10)/((CALADC_12V_85C-CALADC_12V_30C)*10) + 290;
            degF = (degC) * 9 / 5 + 320;
            if (tempUnit == Celsius)  // In Celsius Mode
            {
               deg = degC;
            }
            else
            {
               deg = degF;
            }
            k++;                // Averaging Routine
            sum = sum + deg;
            if(k >= Tsize)
            {

                k=0;
                deg = sum/(Tsize);
                degLED=deg;
                if(tempUnit == Fahrenheit) // In Fahrenheit Mode. Transfer the average value back to degF
                {
                    degLED = deg;  // This is a patch to get T_Control working when in Celsius mode
                }
                else
                {
                    degLED = (deg*9/5) +320; // In Celsius Mode. Recalculate averaged degF
                }

                sum = 0;
                displayTemp();
                update_RGB();
                if(T_Control==TRUE)
                {
                	Temp_Feedback();
                }
            }
}

/********************* Unpack and display each digit on LCD.  Can display up o 999 degrees, but not practically ****/

void displayTemp()
{

    clearLCD();
   if (tempUnit == 0)
    {
        showChar('C',pos6);
    }
    else
    {
       showChar('F',pos6);
    }

    // Handles displaying up to 999.9 degrees
    if (deg>=1000)
        showChar((deg/1000)%10 + '0',pos3);
    if (deg>=100)
        showChar((deg/100)%10 + '0',pos4);
    if (deg>=10)
        showChar((deg/10)%10 + '0',pos5);

    // Degree symbol
    LCDMEM[pos5+1] |= 0x04;

}


void Temp_Feedback()
{

	// Controller goes here
    if(deg < T_SetPoint)
    {
        //turn on heating element
        P2OUT |= BIT0;
    }
    else if(deg > T_SetPoint)
    {
        //turn off heating element
        P2OUT &= ~BIT0;

    }




}




/************************************************** END OF CODE *************************************************/
