/*
 * letimer.c
 *
 *  Created on: April 20, 2018
 *      Author: Nikhil and Shreya
 *  Reference: EFR32 Blue gecko 1 software documentation,AN0004:Clock Management unit,AN0026:Low Energy Timer
 *  Source :https://www.silabs.com/documents/public/example-code/an0026-efm32-letimer.zip
 */

#include "letimer.h"
#include "gpio.h"
#include "cmu.h"
#include "em_core.h"
#include "sleep.h"
#include "lpm.h"
#include "si7021.h"
#include "I2C.h"
#include "main.h"
#include "native_gecko.h"

uint8_t Schedule_event = 0;

void LETIMER_init()
{
	CMU_LETIMER0_Init();

	const LETIMER_Init_TypeDef letimerInit =               //configuration for letimer0
	    {

	        .enable         = true,                 //Starts to count after init is completed
	        .debugRun       = false,                 //Counter would not run while its stopped for debug
	        .comp0Top       = true,                 //COMP0 is used as TOP and loaded into CNT after counter underflow
	        .bufTop         = false,                //it will not load COMP1 into COMP0 when REP0 is 0
	        .out0Pol        = 0,                    //output 0 idle value.
	        .out1Pol        = 0,                    //output 1 idle value.
	        .ufoa0          = letimerUFOANone,      //PWM output is on output 0
	        .ufoa1          = letimerUFOANone,      //Pulse output is on output 1
	        .repMode        = letimerRepeatFree     //keep on Counting until stopped
	    };


		FreqofClock = CMU_ClockFreqGet(cmuClock_LETIMER0); //get clock frequency for clock point, returns current frequency, Freq of ULFCRO is 1000hz

		unsigned int PERIOD_COUNT = (FreqofClock * PERIOD_MSEC)/1000; // the current freq is multiplied by period between blink ie 2.925 secs to get led off period count, count = 2925

	    LETIMER_CompareSet(LETIMER0, 0, PERIOD_COUNT); //  compare registers are set for comp 0

	    unsigned int DELAY_COUNT = (FreqofClock * (PERIOD_MSEC-DELAY_PERIOD_MSEC))/1000; // the current freq is multiplied by the difference of period and delay

	    LETIMER_CompareSet(LETIMER0, 1, DELAY_COUNT); //  compare registers are set for comp 1 to give stabilising delay

	    LETIMER_IntClear(LETIMER0, LETIMER_IFC_UF | LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1); //clear all 3 interrupts

	    LETIMER_IntEnable(LETIMER0, LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1);   // enables interrupts COMP0 for led off time

	    //BlockSleepMode(LETIMER0_EM); // go to the corresponding  after all operation is done

	    NVIC_EnableIRQ(LETIMER0_IRQn); // Enables interrupt

	    /* Initializng LETIMER 0 with the configurations given abboe */
		LETIMER_Init(LETIMER0, &letimerInit);
}


void LETIMER0_IRQHandler(void)
{
	CORE_ATOMIC_IRQ_DISABLE();                       //Disabling interrupts to make the operation atomic

	int intFlags = LETIMER_IntGet(LETIMER0);		// to get pending letimer interrupt flags

	if(intFlags & LETIMER_IF_COMP0)
	{

		LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP0);
		Schedule_event |= LETIMER_EVENT;
	}
	else if(intFlags & LETIMER_IF_COMP1)
	{

		LETIMER_IntClear(LETIMER0,LETIMER_IFC_COMP1);
		Schedule_event |= I2C_STAB_EVENT;				//scheduler for stabilization time after every sensor eneable

	}

	CORE_ATOMIC_IRQ_ENABLE();                         //Re-enabling interrupts
	gecko_external_signal(Schedule_event);

}


