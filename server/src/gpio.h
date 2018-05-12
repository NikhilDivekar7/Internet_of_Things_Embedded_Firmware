//***********************************************************************************
// Include files
//***********************************************************************************
#include "main.h"
#include "em_gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************

// LED0 pin is
#define	LED0_port gpioPortF
#define LED0_pin	4
#define LED0_default	false 	// off
// LED1 pin is
#define LED1_port gpioPortF
#define LED1_pin	5
#define LED1_default	false	// off

#define trigger_port gpioPortD
#define trigger_pin	12
#define trigger_default	true	// off

#define echo_port gpioPortD
#define echo_pin	10
#define echo_default	true	// off

#define PushButton0_PORT 		gpioPortF
#define PushButton0_PIN			6

#define PushButton1_PORT		gpioPortF
#define PushButton1_PIN			7

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_init(void);

