//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef CMU_H_
#define CMU_H_

#include "main.h"
#include "em_cmu.h"

//***********************************************************************************
// function prototypes
//***********************************************************************************
void CMU_init(void);

void CMU_LETIMER0_Init();

void CMU_I2C0_Init();

#endif /*CMU_H_*/
