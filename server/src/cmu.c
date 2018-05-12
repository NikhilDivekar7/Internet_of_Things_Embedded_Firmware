/*
 * cmu.c
 *
 *  Created on: Feb 6, 2018
 *      Author: Nikhil and Shreya.
 *
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"
#include "sleep.h"
#include "em_cmu.h"
//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// function prototypes
//***********************************************************************************
void CMU_init(void)
{
       //CMU_ClockEnable(cmuClock_CORELE, true);                 //Enable clock to Low energy peripheral clock tree
       //CMU_ClockEnable(cmuClock_GPIO, true);   //enable clock for gpio
       //CMU_ClockEnable(cmuClock_HFLE, true);
		//CMU_ClockEnable(cmuClock_GPIO, true);
}

void CMU_LETIMER0_Init()
{
	if(LETIMER0_EM == EM4)
	{
	   CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);    //Enable ULFRCO clock for EM3 1000hz
	   CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); //Select ULFRCO as the clock for LFA clock tree to the letimer
	}
	else
	{

	   CMU_OscillatorEnable(cmuOsc_LFXO, true, true);      //Enable LXF0 clock oscillator, 32768hz
	   CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);   //Select LFXO as the clock for LFA clock tree to the letimer
	   CMU_ClockDivSet(cmuClock_LETIMER0, 2);  //prescaler to divide clock by 4

	}

	CMU_ClockEnable(cmuClock_LETIMER0, true);               //Enable clock to letimer0


}

void CMU_I2C0_Init()
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_I2C0, true);
}

