/*
 * sleep.c
 *
 *  Created on: Mar 30, 2018
 *      Author: Nikhil and Shreya
 *      code reference: ECEN5823 Lecture 3 by Prof Keith Graham
 */


#include "sleep.h"
#include "em_core.h"

void UnblockSleepMode(SleepState minimumMode)
{
    CORE_ATOMIC_IRQ_DISABLE();                            //Disabling interrupts to make the operation atomic
    if(sleep_block_counter[minimumMode] > 0)
    {
        sleep_block_counter[minimumMode]--;           //decreasing the energy mode or unblocking sleep mode
    }
    CORE_ATOMIC_IRQ_ENABLE();                             //Re-enabling interrupts
}

void BlockSleepMode(SleepState minimumMode)
{
	CORE_ATOMIC_IRQ_DISABLE();                            //Disabling interrupts to make the operation atomic
    sleep_block_counter[minimumMode]++;         //increasing the energy mode or blocking sleep mode.
    CORE_ATOMIC_IRQ_ENABLE();                              //Re-enabling interrupts
}
