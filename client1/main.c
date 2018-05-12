/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#include "em_timer.h"

/* Device initialization header */
#include "hal-config.h"

#include "graphics.h"
#include "em_usart.h"

#include "sleep.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#else
#include "bspconfig.h"
#endif

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

#define DISCONNECTED	0
#define SCANNING		1
#define FIND_SERVICE	2
#define FIND_CHAR		3
#define FIND_CHAR2		4
#define GET_SERVICE		5
#define FIND_SERVICE2	7

#define PushButton0_PORT 		gpioPortF
#define PushButton0_PIN			6

#define PushButton1_PORT		gpioPortF
#define PushButton1_PIN			7

#define LIGHT_THRESHOLD			(50)
#define DISTANCE_THRESHOLD		(10)

#define	LED0_port gpioPortF
#define LED0_pin	4
#define LED0_default	false 	// off

#define LED1_port gpioPortF
#define LED1_pin	5
#define LED1_default	false	// off

int light_flag;
// custom service UUId: 148542554e634d728eea133b1a83fdd4
const uint8 serviceUUID[16] = {0xd4, 0xfd, 0x83, 0x1a, 0x3b, 0x13, 0xea, 0x8e, 0x72, 0x4d,
		0x63, 0x4e, 0x55, 0x42, 0x85, 0x14};

// custom char UUId: 01dcdce5969b4691acb4405ed0d83897
const uint8 charUUID1[16] = {0x97, 0x38, 0xd8, 0xd0, 0x5e, 0x40, 0xb4, 0xac, 0x91, 0x46,
		0x9b, 0x96, 0xe5,0xdc, 0xdc, 0x01};

// custom service id 2 4ac1abe5-90cf-4dfb-bdae-e899a141ef09
const uint8 serviceUUID2[16] = {0x09, 0xef, 0x41, 0xa1, 0x99, 0xe8, 0xae, 0xbd, 0xfb, 0x4d,
		0xcf, 0x90, 0xe5, 0xab, 0xc1, 0x4a};

// custom char UUID2: a0bdd6d2-7c7e-4eca-afe3-13b77896e9d7
const uint8 charUUID2[16] = {0xd7, 0xe9, 0x96, 0x78, 0xb7, 0x13, 0xe3, 0xaf, 0xca, 0x4e, 0x7e,
		0x7c, 0xd2, 0xd6, 0xbd, 0xa0};

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
  .config_flags = 0,
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .gattdb = &bg_gattdb_data,
  .ota.flags = 0,
  .ota.device_name_len = 3,
  .ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
  .pa.config_enable = 1, // Enable high power PA
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;

/**
 * @brief  Main function
 */

static uint8 _conn_handle = 0xFF;
static int _main_state;
static uint32 _service_handle;
static uint32 _service_handle1;
static uint16 _char_handle;
static uint16 _char_handle1;
int night_flag = 0;

static void reset_variables()
{
	_conn_handle = 0xFF;
	_main_state = DISCONNECTED;
	_service_handle = 0;
	_char_handle = 0;
}


static int process_scan_response(struct gecko_msg_le_gap_scan_response_evt_t *pResp)
{
	// decoding advertising packets is done here. The list of AD types can be found
	// at: https://www.bluetooth.com/specifications/assigned-numbers/Generic-Access-Profile

    int i = 0;
    int ad_match_found = 0;
	int ad_len;
    int ad_type;

    char name[32];

    while (i < (pResp->data.len - 1))
    {

        ad_len  = pResp->data.data[i];
        ad_type = pResp->data.data[i+1];

        if (ad_type == 0x08 || ad_type == 0x09 )
        {
            // type 0x08 = Shortened Local Name
            // type 0x09 = Complete Local Name
            memcpy(name, &(pResp->data.data[i+2]), ad_len-1);
            name[ad_len-1] = 0;
        }

        // 4880c12c-fdcb-4077-8920-a450d7f9b907
        if (ad_type == 0x06 || ad_type == 0x07)
        {
        	// type 0x06 = Incomplete List of 128-bit Service Class UUIDs
        	// type 0x07 = Complete List of 128-bit Service Class UUIDs
        	if(memcmp(serviceUUID, &(pResp->data.data[i+2]),16) == 0)
        	{
        		ad_match_found = 1;
        	}
        	if(memcmp(serviceUUID2, &(pResp->data.data[i+2]),16) == 0)
			{
				ad_match_found = 1;
			}
        }

        //jump to next AD record
        i = i + ad_len + 1;
    }

    return(ad_match_found);
}

void TimerInitFunc(int duty_cycle)
{
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);

	TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;
	timerCCInit.mode = timerCCModePWM;
	timerCCInit.cmoa = timerOutputActionToggle;

	// Configure CC channel 2
	TIMER_InitCC(TIMER0, 2, &timerCCInit);

	TIMER0->ROUTELOC0 |= TIMER_ROUTELOC0_CC2LOC_LOC26;	//PWM configure
	TIMER0->ROUTEPEN |= TIMER_ROUTEPEN_CC2PEN;

	TIMER_TopSet(TIMER0, 100);

	TIMER0->CMD = TIMER_CMD_START;

	// Set the PWM duty cycle here!
	TIMER_CompareBufSet(TIMER0, 2, duty_cycle);

	// Create a timerInit object, based on the API default
	TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
	timerInit.prescale = timerPrescale256;
	TIMER_Init(TIMER0, &timerInit);

	TIMER0->CMD = TIMER_CMD_STOP;
}

void change_intensity()
{
		CMU_ClockEnable(cmuClock_TIMER0, true);
		CMU_ClockEnable(cmuClock_GPIO, true);
		TIMER0->CMD = TIMER_CMD_STOP;
		TIMER0->ROUTELOC0 = (TIMER0 -> ROUTELOC0 & (~_TIMER_ROUTELOC0_CC2LOC_MASK)) | (TIMER_ROUTELOC0_CC2LOC_LOC26);
		TIMER0->ROUTEPEN = TIMER0->ROUTEPEN | TIMER_ROUTEPEN_CC2PEN;
		TIMER_CompareBufSet(TIMER0, 2, 50);
		TIMER0->CMD = TIMER_CMD_START;
}

void main(void)
{
	int currentval, previousval = 0;
	int flag = 0;
	int vehicle_count = 0;
	char printbuf[128];
	char * returned_value1;
	char * returned_value2;
	char * test3;
	char * confirmid;
	int car_count;

    initMcu();

    initBoard();

    initApp();

    gecko_init(&config);

    GPIO_PinModeSet(PushButton0_PORT, PushButton0_PIN, gpioModeInput, 1);
    GPIO_PinModeSet(PushButton1_PORT, PushButton1_PIN, gpioModeInput, 1);

    GRAPHICS_Init();
    GRAPHICS_Clear();
    GRAPHICS_AppendString("Starting\n");
    GRAPHICS_Update();


    GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
  	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
  	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);
  	//GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

  	TimerInitFunc(30);
  	TIMER0->CMD = TIMER_CMD_START;

  	//change_intensity();

  	struct gecko_msg_le_connection_opened_evt_t * activeConnectionId;
  	while (1)
  	{
    /* Event pointer for handling events */
  		struct gecko_cmd_packet* evt;

    /* Check for stack event. */
  		evt = gecko_wait_event();

    /* Handle events */
    switch (BGLIB_MSG_ID(evt->header)) {
      /* This boot event is generated when the system boots up after reset.
       * Do not call any stack commands before receiving the boot event.
       * Here the system is set to start advertising immediately after boot procedure. */
      case gecko_evt_system_boot_id:

    	  gecko_cmd_sm_delete_bondings();
    	  gecko_cmd_sm_configure(0x05, sm_io_capability_displayyesno);
    	  gecko_cmd_sm_set_bondable_mode(1);
    	  gecko_cmd_le_gap_set_adv_parameters(160,160,7);
    	  gecko_cmd_le_gap_discover(le_gap_discover_generic);
        break;

      case gecko_evt_le_gap_scan_response_id:

          	// process scan responses: this function returns 1 if we found the service we are looking for
          	if(process_scan_response(&(evt->data.evt_le_gap_scan_response)) > 0)
          	{
          		struct gecko_msg_le_gap_open_rsp_t *pResp;
          		// match found -> stop discovery and try to connect
          		gecko_cmd_le_gap_end_procedure();

          		pResp = gecko_cmd_le_gap_open(evt->data.evt_le_gap_scan_response.address, evt->data.evt_le_gap_scan_response.address_type);

          		// make copy of connection handle for later use (for example, to cancel the connection attempt)
          		_conn_handle = pResp->connection;
          	}
          break;

      case gecko_evt_le_connection_opened_id:

      	//	 start service discovery (we are only interested in one UUID)
      	 //gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, serviceUUID);
      	_main_state = FIND_SERVICE;
	  	 activeConnectionId = evt->data.evt_le_connection_opened.connection;

	  	 /* The HTM service typically indicates and indications cannot be given an encrypted property so
	  	  * force encryption immediately after connecting */
	  	 gecko_cmd_sm_increase_security(activeConnectionId);
      	break;


      case gecko_evt_le_connection_closed_id:

          	reset_variables();

          	SLEEP_SleepBlockEnd(sleepEM2); // enable sleeping after disconnect

          	gecko_cmd_le_gap_discover(le_gap_discover_generic);
         break;



       case gecko_evt_gatt_service_id:

          	if(evt->data.evt_gatt_service.uuid.len == 16)
          	{
          		if(memcmp(serviceUUID, evt->data.evt_gatt_service.uuid.data,16) == 0)
          		{
          			_service_handle = evt->data.evt_gatt_service.service;
          		}
          		if(memcmp(serviceUUID2, evt->data.evt_gatt_service.uuid.data,16) == 0)
				{
					_service_handle1 = evt->data.evt_gatt_service.service;
				}
          	}
       break;

       case gecko_evt_sm_confirm_passkey_id:		//MITM protection

    	   confirmid = (char *)malloc(5*sizeof(char));
    	   itoa(evt->data.evt_sm_confirm_passkey.passkey,confirmid, 10);
    	   GRAPHICS_Clear();
    	   GRAPHICS_AppendString(confirmid);
    	   GRAPHICS_Update();

    	   //acknowledging MITM passkey with push button
    	   while(GPIO_PinInGet(PushButton0_PORT, PushButton0_PIN) == 1) && (GPIO_PinInGet(PushButton1_PORT, PushButton1_PIN) == 1);
    	   if(GPIO_PinInGet(PushButton0_PORT, PushButton0_PIN) == 0)
    	   {
    		   gecko_cmd_sm_passkey_confirm(evt->data.evt_sm_confirm_passkey.connection,1);
    	       GRAPHICS_AppendString("\n Bonding confirmed");
    	       GRAPHICS_Update();
    	   }
    	   else if(GPIO_PinInGet(PushButton1_PORT, PushButton1_PIN) == 0)
    	   {
    		   gecko_cmd_sm_passkey_confirm(evt->data.evt_sm_confirm_passkey.connection,0);
    	       GRAPHICS_AppendString("\n Bonding failed");
    	       GRAPHICS_Update();
    	       gecko_cmd_sm_delete_bondings();
    	       gecko_cmd_le_gap_discover(le_gap_discover_generic);
    	   }
    	   break;
       break;

       case gecko_evt_sm_bonded_id:
    	   //Bond success
           	  GRAPHICS_AppendString("Bonded /n");
           	  GRAPHICS_Update();
           	  gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, serviceUUID);
           	  break;

             case gecko_evt_sm_bonding_failed_id:
         	  	  GRAPHICS_Clear();
         	  	  GRAPHICS_AppendString("Bonding failed \n");
         	  	  GRAPHICS_Update();
         	  	  gecko_cmd_sm_delete_bondings();
         	  	  gecko_cmd_le_gap_discover(le_gap_discover_generic);
         	  	  break;

       case gecko_evt_gatt_procedure_completed_id:

    	   //state machine to get 2 services and their characteristics
           	switch(_main_state)
           	{
           	case FIND_CHAR2:
			if (_char_handle1 > 0)
			{
				// Char found, turn on indications
				gecko_cmd_gatt_set_characteristic_notification(_conn_handle, _char_handle1, gatt_notification);
				_main_state = FIND_SERVICE;
			}
			else
			{
				gecko_cmd_endpoint_close(_conn_handle);
			}
			break;

           	case FIND_SERVICE:

			if (_service_handle > 0)
			{
				// Service found, next search for characteristics
				gecko_cmd_gatt_discover_characteristics(_conn_handle, _service_handle);
				_main_state = FIND_CHAR;
			}
			else
			{
				gecko_cmd_endpoint_close(_conn_handle);
			}

			break;

           	case FIND_SERVICE2:
           	gecko_cmd_gatt_discover_primary_services_by_uuid(_conn_handle, 16, serviceUUID2);
			_main_state = GET_SERVICE;
			break;

           	case GET_SERVICE:
           		if(_service_handle1 > 0)
           		{
           			gecko_cmd_gatt_discover_characteristics(_conn_handle, _service_handle1);
           			_main_state = FIND_CHAR2;
           		}
           		else
           		{
           			// no service found -> disconnect
           			gecko_cmd_endpoint_close(_conn_handle);
           		}
			break;

           	case FIND_CHAR:
           		if (_char_handle > 0)
           		{
           			// Char found, turn on indications
           			gecko_cmd_gatt_set_characteristic_notification(_conn_handle, _char_handle, gatt_notification);
           			_main_state = FIND_SERVICE2;
           		}
           		else
           		{
           			gecko_cmd_endpoint_close(_conn_handle);
           		}
           		break;

           	default:
           		break;
           	}
           	break;

       case gecko_evt_gatt_characteristic_id:

          		if(evt->data.evt_gatt_characteristic.uuid.len == 16)
          		{
          			if(memcmp(charUUID1, evt->data.evt_gatt_characteristic.uuid.data,16) == 0)
          			{

          				returned_value1 = (char *)malloc(sizeof(char)*16);
          				_char_handle = evt->data.evt_gatt_characteristic.characteristic;
          			}
          			else if(memcmp(charUUID2, evt->data.evt_gatt_characteristic.uuid.data,16) == 0)
					{

						returned_value2 = (char *)malloc(sizeof(char)*16);
						_char_handle1 = evt->data.evt_gatt_characteristic.characteristic;
					}
          		}
        break;

          	case gecko_evt_gatt_characteristic_value_id:

          		//light service
          		if(evt->data.evt_gatt_characteristic_value.characteristic == _char_handle)
          		{
          			//int lux_value = evt->data.evt_gatt_characteristic_value.value.data;
          			// data received from SPP server -> print to UART
          			// NOTE: this works only with text (no binary) because printf() expects null-terminated strings as input
          			int retvalue;
          			memcpy(printbuf, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
          			retvalue = evt->data.evt_gatt_characteristic_value.value.data;
          			printbuf[evt->data.evt_gatt_characteristic_value.value.len] = 0;
          			returned_value1 = (char *)malloc(sizeof(char)*1);
          			itoa(*(evt->data.evt_gatt_characteristic_value.value.data),returned_value1,10);

          			GRAPHICS_Clear();
          			GRAPHICS_AppendString(returned_value1);
          			GRAPHICS_Update();

          			//checking luminousity
          			if(*evt->data.evt_gatt_characteristic_value.value.data < 20) //night time
					{
						int i;
						GPIO_PinOutSet(LED0_port, LED0_pin);
						if(night_flag == 0)
						{
						//TimerInitFunc(25);
						//TIMER0->CMD = TIMER_CMD_START;
						}
						night_flag = 1;
						light_flag = 1;
						GRAPHICS_AppendString("Night time \n");
						GRAPHICS_Update();
					}
					else if (*evt->data.evt_gatt_characteristic_value.value.data > 20)  //day time
					{
						GPIO_PinOutClear(LED0_port,LED0_pin);
						GRAPHICS_AppendString("Day time \n");
						light_flag = 0;
						GRAPHICS_Update();
					}
          			gecko_cmd_gatt_discover_characteristics(_conn_handle, _service_handle1);
          		}

          		//ultrasonic data
          		if(evt->data.evt_gatt_characteristic_value.characteristic == _char_handle1)
			{
				// data received from SPP server -> print to UART
				// NOTE: this works only with text (no binary) because printf() expects null-terminated strings as input
          			uint8_t retval2;
          			memcpy(printbuf, evt->data.evt_gatt_characteristic_value.value.data, evt->data.evt_gatt_characteristic_value.value.len);
          			retval2 = evt->data.evt_gatt_characteristic_value.value.data;
          			printbuf[evt->data.evt_gatt_characteristic_value.value.len] = 0;
          			returned_value2 = (char *)malloc(sizeof(char)*1);
          			int ultra_value = evt->data.evt_gatt_characteristic_value.value.data;
          			itoa(*(evt->data.evt_gatt_characteristic_value.value.data),returned_value2,10);
				//GRAPHICS_Clear();
//          		          			char buf[40] = {0};
//          		          			snprintf(buf,40,"Client: %d\n", final_value);
				//GRAPHICS_AppendString("Night Time");
			//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
					//GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);
			if(light_flag == 1)		//night so ultrasonic working
			{
				GRAPHICS_AppendString(returned_value2);
				GRAPHICS_Update();
				//char buf[16] = {0};
				//snprintf(buf,16,"Distance:%d cm\n", distance);
				//GRAPHICS_AppendString(buf);
				//GRAPHICS_Update();
				UDELAY_Delay(1000000);
				previousval = 0;

						//obsatcle/vehicle condition
						if(*(evt->data.evt_gatt_characteristic_value.value.data) < DISTANCE_THRESHOLD)
						{
								CMU_ClockEnable(cmuClock_GPIO, true);
								currentval =1;
								flag = 1;

								GPIO_PinOutSet(LED1_port,LED1_pin);
								GPIO_PinOutClear(LED0_port, LED0_pin);
								GRAPHICS_AppendString("Obstacle detected \n");
								GRAPHICS_Update();
						}
						else
						{
								GPIO_PinOutClear(LED1_port,LED1_pin);
								GPIO_PinOutSet(LED0_port, LED0_pin);
								GRAPHICS_AppendString("No obstacle\n");
								currentval = 0;
								GRAPHICS_Update();
						}
						{
							if(currentval == 0 && flag == 1)		//one car detected only once
							{
								vehicle_count++;
								car_count = vehicle_count;
								char buf1[16] = {0};
								/*snprintf(buf1,16,"Car count:%d\n", vehicle_count);
								GRAPHICS_AppendString(buf1);
								GRAPHICS_Update();*/
								flag = 0;
							}
						}
					}

			}
          	break;

      default:
        break;
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
