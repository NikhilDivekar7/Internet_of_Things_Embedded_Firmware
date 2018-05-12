//***********************************************************************************
// Include files
//***********************************************************************************

#include <stdint.h>
#include <stdbool.h>



//***********************************************************************************
// defined files
//***********************************************************************************



//***********************************************************************************
// global variables
//***********************************************************************************
#define MIN_ADVERTISING_INTERVAL_MS		(337)
#define MAX_ADVERTISING_INTERVAL_MS		(337)
#define MIN_ADVERTISING_INTERVAL_COUNT	(539)			//count = MIN_ADVERTISING_INTERVAL_MS * 1.6
#define MAX_ADVERTISING_INTERVAL_COUNT	(539)			//count = MAX_ADVERTISING_INTERVAL_MS * 1.6
#define MIN_CONNECTION_INTERVAL_MS		(75)
#define MAX_CONNECTION_INTERVAL_MS		(75)
#define MIN_CONNECTION_INTERVAL_COUNT	(60)			//Count = MIN_CONNECTION_INTERVAL_MS/1.25
#define MAX_CONNECTION_INTERVAL_COUNT	(60)			//Count = MAX_CONNECTION_INTERVAL_MS/1.25
#define SLAVE_LATENCY_MS				(450)
#define SLAVE_LATENCY					(5)				//Latency = (SLAVE_LATENCY_MS/MAX_CONNECTION_INTERVAL_MS) - 1
#define CONNECTION_TIMEOUT_MS			(250)			//Timeout >= (latency_ms*2)
#define TX_MAX							(80)
#define TX_MIN							(-260)

//***********************************************************************************
// function prototypes
//***********************************************************************************

