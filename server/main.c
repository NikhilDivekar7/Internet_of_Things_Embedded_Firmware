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
#include "ble-configuration.h"
#include "board_features.h"
#include "infrastructure.h"
#include <time.h>

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
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
//***********************************************************************************
// Include files
//***********************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "main.h"
#include "gpio.h"
#include "sleep.h"
#include "letimer.h"
#include "cmu.h"
#include "I2C.h"
#include "si7021.h"
#include "lpm.h"
#include "em_core.h"
#include "graphics.h"
#include "light.h"
#include "em_timer.h"

#include <unistd.h>
#include <sys/time.h>

struct timespec start, end;
float diff;
int light_flag= 0;

//***********************************************************************************
// defined files
//***********************************************************************************
#define key 0x4005




//***********************************************************************************
// global variables
//***********************************************************************************
int8_t rssi;
int vehicle_count = 0;
int currentval, previousval = 0;
int flag = 0;

//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************
static struct ps_streetlight {
	int8_t count_of_vehicles;
} ps_streetlight;

static int persistent_state_load(void)
{
  struct gecko_msg_flash_ps_load_rsp_t* pLoad;

  pLoad = gecko_cmd_flash_ps_load(key);

  if (pLoad->result)
  {
    return -1;
  }

  memcpy(&ps_streetlight, pLoad->value.data, pLoad->value.len);

  return 0;
}

static int persistent_state_store(void)
{
  struct gecko_msg_flash_ps_save_rsp_t* pSave;

  pSave = gecko_cmd_flash_ps_save(key, sizeof(struct ps_streetlight), (const uint8*)&ps_streetlight);

  if (pSave->result)
  {
        return(-1);
  }

  return 0;
}


void lightMeasureAndPush()
{
	uint8_t htmTempBuffer[5] = {0}; /* Stores the temperature data in the Health Thermometer (HTM) format. */
	uint8_t flags = 0x00;/* HTM flags set as 0 for Celsius, no time stamp and yes temperature type. */
	float  light_lumen = 0;     /* Stores the Temperature data read from the RHT sensor. */
	int light_int;
	uint32_t lux;   /* Stores the temperature data read from the sensor in the correct format */
	uint8_t *p = htmTempBuffer; /* Pointer to HTM temperature buffer needed for converting values to bitstream. */

	light_lumen = getLuminosityValue();
	light_int = (int)light_lumen;
	GRAPHICS_Clear();
	char buf[16] = {0};
	snprintf(buf,16,"Luminosity: %d\n",light_int);
	GRAPHICS_AppendString(buf);
	GRAPHICS_Update();

	if(light_lumen < LIGHT_THRESHOLD)
	{
		CMU_ClockEnable(cmuClock_GPIO, true);
		light_flag = 1;
		GRAPHICS_AppendString("Night time \n");
		GRAPHICS_Update();
	}
	else
	{
		GPIO_PinOutClear(LED1_port,LED1_pin);
		GRAPHICS_AppendString("Day time \n");
		light_flag = 0;
		GRAPHICS_Update();
	}

	/* Send indication of the temperature in htmTempBuffer to all "listening" clients.
	 * This enables the Health Thermometer in the Blue Gecko app to display the temperature.
	 *  0xFF as connection ID will send indications to all connections. */

	 gecko_cmd_gatt_server_send_characteristic_notification( 0xFF, gattdb_light, 1, &light_int);
}

//***********************************************************************************
// main
//***********************************************************************************

/**
 * @brief  Main function
 */

void ultra()
{
		GPIO_PinOutClear(trigger_port, trigger_pin);
		UDELAY_Delay(10);
		GPIO_PinOutSet(trigger_port, trigger_pin);
		UDELAY_Delay(10);
		GPIO_PinOutClear(trigger_port, trigger_pin);
		while(GPIO_PinInGet(echo_port, echo_pin) == 0);
		long int count = 0;
		while(GPIO_PinInGet(echo_port, echo_pin) != 0)
		{
			count++;
		}

		FreqofClock = CMU_ClockFreqGet(cmuClock_GPIO);

		float PERIOD_USEC = (count * 1000000) /FreqofClock; //period in micro seconds
		int distance = (int)((343/100)*PERIOD_USEC)/2; //distance in cms
		gecko_cmd_gatt_server_send_characteristic_notification( 0xFF, gattdb_Ultra, 1, &distance);

		if(light_flag == 1)
		{
		char buf[16] = {0};
		snprintf(buf,16,"Distance:%d cm\n", distance);
		GRAPHICS_AppendString(buf);
		GRAPHICS_Update();
		UDELAY_Delay(1000000);
		previousval = 0;
		if(distance < DISTANCE_THRESHOLD)
		{
				CMU_ClockEnable(cmuClock_GPIO, true);
				currentval =1;
				flag = 1;

				GPIO_PinOutSet(LED1_port,LED1_pin);
				GRAPHICS_AppendString("Obstacle detected \n");
				GRAPHICS_Update();
		}
		else
		{
				GPIO_PinOutClear(LED1_port,LED1_pin);
				GRAPHICS_AppendString("No obstacle\n");
				currentval = 0;
				GRAPHICS_Update();
		}
		if(currentval == 0 && flag == 1)
		{
			vehicle_count++;
			persistent_state_store();
			char buf1[16] = {0};
			snprintf(buf1,16,"Car count:%d\n", vehicle_count);
			GRAPHICS_AppendString(buf1);
			GRAPHICS_Update();
			flag = 0;
			int a = 7;
			gecko_cmd_gatt_server_send_characteristic_notification( 0xFF, gattdb_count, 1, &vehicle_count);
			gecko_cmd_gatt_server_send_characteristic_notification( 0xFF, gattdb_alert, 1, &a);
		}
	}
}

int main(void)
{
	char * displayid;
	char * confirmid;
	int accept, reject;
	// Initialize device
	initMcu();

	// Initialize board
	initBoard();

	// Initialize stack
	gecko_init(&config);

	// Initialize clocks
	CMU_init();
	CMU_LETIMER0_Init();

	/* Initialize GPIO */
	gpio_init();

	setup_I2C();
	LETIMER_init();
	GRAPHICS_Init();
	if(persistent_state_load() == -1)
	{
		GRAPHICS_Clear();
		GRAPHICS_AppendString("Storing Value for first time\n");
		GRAPHICS_Update();
		ps_streetlight.count_of_vehicles = 0;
		persistent_state_store();
	}

	struct gecko_msg_le_connection_opened_evt_t * activeConnectionId;

	UDELAY_Delay(1000000);

	persistent_state_load();

	GPIO_PinModeSet(trigger_port, trigger_pin, gpioModePushPull, trigger_default);
	GPIO_PinModeSet(echo_port, echo_pin, gpioModeInputPullFilter, echo_default);

	while (1)
	{
	    /*GRAPHICS_Clear();
	      char buf[16] = {0};
	      snprintf(buf,16,"Distance: %d\n", distance);
	      GRAPHICS_AppendString(buf);
	      GRAPHICS_Update();*/
		 /* Event pointer for handling events */
		    struct gecko_cmd_packet* evt;

		    /* Check for stack event. */
		    evt = gecko_wait_event();

		    /* Handle events */
		    switch (BGLIB_MSG_ID(evt->header))
		    {
		      /* This boot event is generated when the system boots up after reset.
		       * Do not call any stack commands before receiving the boot event.
		       * Here the system is set to start advertising immediately after boot procedure. */
		      case gecko_evt_system_boot_id:
				gecko_cmd_system_set_tx_power(0);
				gecko_cmd_sm_delete_bondings();
				gecko_cmd_sm_configure(0x05, sm_io_capability_displayyesno); /* Numeric comparison */
				gecko_cmd_sm_set_bondable_mode(1);
				ps_streetlight.count_of_vehicles++;
				gecko_cmd_hardware_set_soft_timer(32768, 0, 0);

		        /* Set advertising parameters. 337ms advertisement interval. All channels used.
		         * The first two parameters are minimum and maximum advertising interval, both in
		         * units of (milliseconds * 1.6). The third parameter '7' sets advertising on all channels. */
		        gecko_cmd_le_gap_set_adv_parameters(MIN_ADVERTISING_INTERVAL_COUNT, MAX_ADVERTISING_INTERVAL_COUNT,  7);

		        /* Start general advertising and enable connections. */
		        gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
		        
		        break;

		      case gecko_evt_sm_passkey_display_id:
		    	displayid = (char *)malloc(5*sizeof(char));
		      	itoa(evt->data.evt_sm_passkey_display.passkey, displayid, 10);
		      	break;

		      case gecko_evt_sm_confirm_passkey_id:
			  confirmid = (char *)malloc(5*sizeof(char));
			  itoa(evt->data.evt_sm_confirm_passkey.passkey,confirmid, 10);
			  gecko_cmd_sm_passkey_confirm(evt->data.evt_sm_confirm_passkey.connection,1);
			  GRAPHICS_Clear();
			  GRAPHICS_AppendString("\n confirmed");
			  GRAPHICS_Update();
			  bool read_y_n = true;
				  break;

		      case gecko_evt_le_connection_closed_id:

		        /* Check if need to boot to dfu mode */
		        if (boot_to_dfu) {
		          /* Enter to DFU OTA mode */
		          gecko_cmd_system_reset(2);
		        } else {
		        gecko_cmd_system_set_tx_power(0);
		          /* Restart advertising after client has disconnected */
		          gecko_cmd_le_gap_set_mode(le_gap_general_discoverable, le_gap_undirected_connectable);
		        }
		        break;

		      case gecko_evt_hardware_soft_timer_id:
		    	  LoadPowerON();
		    	  lum_enable();
		    	  lightMeasureAndPush();
		    	  LoadPowerOFF();
		          ultra();
		          break;


		      case gecko_evt_le_connection_opened_id:

		          	  gecko_cmd_le_connection_set_parameters(evt->data.evt_le_connection_opened.connection,
		          			  	  	  	  	  	  	  	  	  MIN_CONNECTION_INTERVAL_COUNT,
		      												  MAX_CONNECTION_INTERVAL_COUNT,
		      												  SLAVE_LATENCY,
		      												  CONNECTION_TIMEOUT_MS);
		          	activeConnectionId = evt->data.evt_le_connection_opened.connection;
		          			          	gecko_cmd_sm_increase_security(activeConnectionId);
		          			          	gecko_cmd_sm_enter_passkey(evt->data.evt_sm_passkey_request.connection,123456);

		          	  break;

		      /* Events related to OTA upgrading
		         ----------------------------------------------------------------------------- */

		      /* Check if the user-type OTA Control Characteristic was written.
		       * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
		      case gecko_evt_gatt_server_user_write_request_id:

		        if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
		          /* Set flag to enter to OTA mode */
		          boot_to_dfu = 1;
		          /* Send response to Write Request */
		          gecko_cmd_gatt_server_send_user_write_response(
		            evt->data.evt_gatt_server_user_write_request.connection,
		            gattdb_ota_control,
		            bg_err_success);

		          /* Close connection to enter to DFU OTA mode */
		          gecko_cmd_endpoint_close(evt->data.evt_gatt_server_user_write_request.connection);
		        }
		        break;


		      case gecko_evt_gatt_server_characteristic_status_id:
		    	  //if (evt-> data.evt_gatt_server_characteristic_status.status_flags == gatt_server_confirmation)
		    	      	  {
		    	      		  gecko_cmd_le_connection_get_rssi(evt-> data.evt_gatt_server_characteristic_status.connection);
		    	      	  }
		    	          break;
		              break;


		      case gecko_evt_system_external_signal_id:
		    	  if((evt->data.evt_system_external_signal.extsignals & I2C_STAB_EVENT) != 0)
				  {
					if(TempSensor_Enabled())
					{
//						LoadPowerON();
//						lum_enable();
//						lightMeasureAndPush();
//						LoadPowerOFF();
					}
					CORE_AtomicDisableIrq();
					Schedule_event &= ~I2C_STAB_EVENT;
					CORE_AtomicEnableIrq();
				  }
		    	  if((evt->data.evt_system_external_signal.extsignals & LETIMER_EVENT) != 0)
		    	  {
		    		GPIO_PinModeSet(gpioPortD, 9, gpioModePushPull,1) ;
		    		CORE_AtomicDisableIrq();
		    		Schedule_event &= ~LETIMER_EVENT;
		    		CORE_AtomicEnableIrq();
		    	  }


		    	  break;


		      case gecko_evt_le_connection_rssi_id:


		      			rssi = evt->data.evt_le_connection_rssi.rssi;
		      			struct gecko_msg_system_set_tx_power_rsp_t *tx_rsp;
		      			if(rssi > -35)
		      				tx_rsp = gecko_cmd_system_set_tx_power(TX_MIN);
		      			else if(( rssi < -35 ) && (rssi > -45))
		      				tx_rsp = gecko_cmd_system_set_tx_power(-200);
		      			else if((rssi < -45 ) && (rssi > -55))
		      				tx_rsp = gecko_cmd_system_set_tx_power(-150);
		      			else if((rssi < -55) && (rssi > -65))
		      				tx_rsp = gecko_cmd_system_set_tx_power(-50);
		      			else if((rssi < -65) && (rssi > -75))
		      				tx_rsp = gecko_cmd_system_set_tx_power(0);
		      			else if((rssi < -75) && (rssi > -85))
		      				tx_rsp = gecko_cmd_system_set_tx_power(50);
		      			else
		      				tx_rsp = gecko_cmd_system_set_tx_power(TX_MAX);
		      			int16 power = tx_rsp->set_power;
		      			break;

		      default:
		        break;
		    }
	}
}






/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
