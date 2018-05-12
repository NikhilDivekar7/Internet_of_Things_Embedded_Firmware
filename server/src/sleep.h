/*
 * sleep.h
 *
 *  Created on: Jan 30, 2018
 *  Author: Shreya
 *  Code Reference: ECEN5823 lecture 3 by Prof Keith Graham
 */

#ifndef SLEEP_H_
#define SLEEP_H_
#define NumberofEMs 4       //macro for  Energy modes in the blue Gecko

#include "em_emu.h" 		// to include energy mgmt unit functions


#define LETIMER0_EM		(EM4)       //Macro for Minimum energy mode for the system, change this to change the energy modes

unsigned char sleep_block_counter[NumberofEMs];  //Global of array sleep block counter for each energy mode

typedef enum {          //Enum for Energy Modes
    EM0 = 0,
    EM1 = 1,
    EM2 = 2,
    EM3 = 3,
    EM4 = 4
}SleepState;

void UnblockSleepMode(SleepState minimumMode);	//Function to unblock sleep modes

void BlockSleepMode(SleepState minimumMode); //Function to block sleep modes

__STATIC_INLINE void Sleep(void)   //Function to put the SoC to the minimum permitted sleep mode
{
    if(sleep_block_counter[0]>0)              //If EM0 blocked, do nothing
        return;
    else if(sleep_block_counter[1]>0)     //If EM1 blocked, enter EM0
    	return;
        //EMU_EnterEM1();
    else if(sleep_block_counter[2]>0)     //If EM2 blocked, enter EM1
    	EMU_EnterEM1();
        //EMU_EnterEM2(true);
    else if(sleep_block_counter[3]>0)     //If EM3 blocked, enter EM2
    	EMU_EnterEM2(true);
        //EMU_EnterEM3(true);
    else                                    //If there are no blocks, go to EM3, avoid entering EM4
    	EMU_EnterEM3(true);
    	return;
}




#endif /* SLEEP_H_ */
