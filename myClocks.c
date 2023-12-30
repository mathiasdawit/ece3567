// ----------------------------------------------------------------------------
// myClocks.c  ('FR6989 Launchpad)
//
// This routine sets up the Low Frequency crystal (LFXT) and high-freq
// internal clock source (DCO). Then configures ACLK, SMCLK, and MCLK:
//    ACLK  = 32KHz
//    SMCLK =  8MHz
//    MCLK  =  8MHz
//
// Gregg Chapman, The Ohio State University, February 2018
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "ECE3567.h"


//***** Defines ***************************************************************
// See additional #defines in 'myClocks.h'


//***** Global Variables ******************************************************
uint32_t myACLK  = 0;
uint32_t mySMCLK = 0;
uint32_t myMCLK  = 0;




/* void Init_Timer_A0(void)
{
//***********************  Configure Timer A0 for a 10 Hz interrupt ***************************
      TA0CCR0 = 0x0CCE;    // for 10 Hz=100 ms and count is 3278
      TA0CTL = 0x0110;    // Set ACLK, UP MODE for Timer_0
      TA0CCTL0 |= CCIE;   // Enable interrupt for Timer_0
      TA0CCTL1 = 0x00E0;  // CCR1 reset/set
      TA0CCR1 = 0x0667;    // CCR1 PWM at 50 % duty cycle, on TA0.1
      P1DIR  |= 0x40;
      P1SEL0 |=0x40;
      P1SEL1 |= 0x40;
      return;
}
*/


void Init_Timer_A0(void)
{
    //***********************  Configure Timer A0 for a 16 Hz interrupt ***************************
      TA0CCR0 = 0x07FF;    // for 16 Hz=62.5 ms and count is 2047
      TA0CTL = 0x0110;    // Set ACLK, UP MODE for Timer_0
      TA0CCTL0 |= CCIE;   // Enable interrupt for Timer_0
      TA0CCTL1 = 0x04E0;  // CCR1 reset/set
      TA0CCR1 = 0x03FF;    // CCR1 PWM at 50 % duty cycle, on TA0.1
      P1DIR  |= 0x40;       // Configure P1.6 as TA0.1 output (OUT1)
      P1SEL0 |=0x40;
      P1SEL1 |= 0x40;
      return;
}


//***** initClocks ************************************************************
void Init_Clocks(void) {

//    Init_Command_Timer();



    //**************************************************************************
    // Configure Oscillators
    //**************************************************************************
    // Set the LFXT and HFXT crystal frequencies being used so that driverlib
    //   knows how fast they are (needed for the clock 'get' functions)
    CS_setExternalClockSource(
            LF_CRYSTAL_FREQUENCY_IN_HZ,
            HF_CRYSTAL_FREQUENCY_IN_HZ
    );

    // Verify if the default clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();

    // Initialize LFXT crystal oscillator without a timeout. In case of failure
    //   the code remains 'stuck' in this function.
    // For time-out instead of code hang use CS_turnOnLFXTWithTimeout(); see an
    //   example of this in lab_04c_crystals.
    // Note that this requires PJ.6 and PJ.7 pins to be connected (and configured) 
    //   as clock input pins.
    CS_turnOnLFXT( CS_LFXT_DRIVE_0 );



    //**************************************************************************
    // Configure Clocks
    //**************************************************************************
    // Set ACLK to use LFXT as its oscillator source (32KHz)
    // With a 32KHz crystal and a divide by 1, ACLK should run at that rate
    CS_initClockSignal(
            CS_ACLK,                                                            // Clock you're configuring
            CS_LFXTCLK_SELECT,                                                  // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Set SMCLK to use DCO as its oscillator source (DCO was configured earlier in this function for 8MHz)
    CS_initClockSignal(
            CS_SMCLK,                                                           // Clock you're configuring
            CS_DCOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Set MCLK to use DCO as its oscillator source (DCO was configured earlier in this function for 8MHz)
    CS_initClockSignal(
            CS_MCLK,                                                            // Clock you're configuring
            CS_DCOCLK_SELECT,                                                   // Clock source
            CS_CLOCK_DIVIDER_1                                                  // Divide down clock source by this much
    );

    // Verify that the modified clock settings are as expected
    myACLK  = CS_getACLK();
    mySMCLK = CS_getSMCLK();
    myMCLK  = CS_getMCLK();
}
/************************************************** END OF CODE *************************************************/




