// ----------------------------------------------------------------------------
// myGpio.c  ('FR6989 Launchpad)
// Gregg Chapman, The Ohio State University, February 2018
// ----------------------------------------------------------------------------

//***** Header Files **********************************************************
#include <driverlib.h>
#include "ECE3567.h"
#include "stdio.h"

//*****************************************************************************
// Initialize GPIO
//*****************************************************************************
void Init_GPIO(void) {

    GPIO_setAsPeripheralModuleFunctionOutputPin(
    		myUart.Channels[0].TxPort,
    		myUart.Channels[0].TxPin,
    		myUart.Channels[0].TxSel
	);
    GPIO_setAsPeripheralModuleFunctionOutputPin(
       		myUart.Channels[0].RxPort,
        	myUart.Channels[0].RxPin,
        	myUart.Channels[0].RxSel
	);

    // EUSCI_UART_A1 pin configuration
    GPIO_setAsPeripheralModuleFunctionOutputPin(
    		myUart.Channels[1].TxPort,
    		myUart.Channels[1].TxPin,
    		myUart.Channels[1].TxSel
	);
    GPIO_setAsPeripheralModuleFunctionOutputPin(
       		myUart.Channels[1].RxPort,
        	myUart.Channels[1].RxPin,
        	myUart.Channels[1].RxSel
	);

    // Set LFXT (low freq crystal pins) to crystal input (rather than GPIO)
    // Since HFXT is not used, we don't need to set these pins. But for the 
    // record, they are:
    //              GPIO_PIN6                            // HFXTIN on PJ.6
    //              GPIO_PIN7                            // HFXOUT on PJ.7
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_PJ,
            GPIO_PIN4 +                                  // LFXIN  on PJ.4
            GPIO_PIN5 ,                                  // LFXOUT on PJ.5
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    // Set all GPIO pins to output low to prevent floating input and reduce power consumption
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);

    // Set P4.1 and P4.2 as Secondary Module Function Input, LFXT.
    GPIO_setAsPeripheralModuleFunctionInputPin(
           GPIO_PORT_PJ,
           GPIO_PIN4 + GPIO_PIN5,
           GPIO_PRIMARY_MODULE_FUNCTION
           );

    //********************************  The RED and GREEN LEDs are useful for Troubleshooting **********************/

    // Set pin P1.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );                             // Red LED (LED1)
    GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Set pin P9.7 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P9, GPIO_PIN7 );                             // Green LED (LED2)
    GPIO_setOutputLowOnPin( GPIO_PORT_P9, GPIO_PIN7 );


}

/************************************************** END OF CODE *************************************************/
