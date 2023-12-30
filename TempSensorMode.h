/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 */

/*******************************************************************************
 *
 * TempSensorMode.h
 *
 * Simple thermometer application that uses the internal temperature sensor to
 * measure and display die temperature on the segmented LCD screen
 *
 * February 2015
 * E. Chen
 * Modified - Gregg Chapman, The Ohio State University, February 2018
 *
 ******************************************************************************/

#ifndef OUTOFBOX_MSP430FR6989_TEMPSENSORMODE_H_
#define OUTOFBOX_MSP430FR6989_TEMPSENSORMODE_H_
#define TEMPSENSOR_MODE       2
// See device datasheet for TLV table memory mapping
#define CALADC_12V_30C  *((unsigned int *)0x1A1A)       // Temperature Sensor Calibration-30 C
#define CALADC_12V_85C  *((unsigned int *)0x1A1C)       // Temperature Sensor Calibration-85 C


#endif /* OUTOFBOX_MSP430FR6989_TEMPSENSORMODE_H_ */

/************************************************** END OF CODE *************************************************/
