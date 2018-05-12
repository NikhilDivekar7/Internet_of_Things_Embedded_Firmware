
#include "light.h"
#include "em_gpio.h"
#include <math.h>

void I2C0_init(void)
{
	CMU_ClockEnable(cmuClock_HFPER,true);
	CMU_ClockEnable(cmuClock_I2C0, true);

	//set gpio pin modes and using wiredAnd mode for the Pull ups
	GPIO_PinModeSet(gpioPortC, 10, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(gpioPortC, 11, gpioModeWiredAnd, 1);

	I2C_Init_TypeDef i2c_init_t = I2C_INIT_DEFAULT;
	i2c_init_t.clhr = i2cClockHLRStandard;
	i2c_init_t.master = true;
	i2c_init_t.refFreq = 0;
	i2c_init_t.freq = 5000; //normal mode
	i2c_init_t.enable = false; //will start when required. Not on Init

	//routing the pins to specific location
	I2C0 -> ROUTELOC0 = I2C_ROUTELOC0_SDALOC_LOC16 |I2C_ROUTELOC0_SCLLOC_LOC14;
	I2C0 -> ROUTEPEN = I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPEN;

	//init I2C
	I2C_Init(I2C0, &i2c_init_t);

	//reset the I2C slave
	for (int i=0; i<9; i++){
		GPIO_PinOutClear(gpioPortC, 10);
		GPIO_PinOutSet(gpioPortC, 10);
	}

	//reset the I2C bus
	if(I2C0->STATE & I2C_STATE_BUSY){
		I2C0->CMD = I2C_CMD_ABORT;
	}
}


uint8_t I2C0_readByte(uint8_t slave_addr,uint8_t reg_addr)
{
	I2C0->CMD |= I2C_IFC_MSTOP;
	I2C0->CMD |= I2C_IFC_SSTOP;
	I2C0->CMD |= I2C_IFC_TXC;

	uint8_t read_data;
	I2C0->TXDATA = (slave_addr << 1) ;//start with write

	I2C0->CMD = I2C_CMD_START; //start

	while ((I2C0->IF & I2C_IF_ACK) == 0); //wait for response

	I2C0->IFC = I2C_IFC_ACK; //clear ACK

	I2C0->TXDATA = reg_addr;

	while ((I2C0->IF & I2C_IF_ACK) == 0);

	I2C0->IFC = I2C_IFC_ACK;

	I2C0->CMD = I2C_CMD_START;		//restart

	I2C0->TXDATA = (slave_addr << 1) | 0x01 ;//read

	while ((I2C0->IF & I2C_IF_ACK) == 0);

	I2C0->IFC = I2C_IFC_ACK;

	while((I2C0->IF & I2C_IF_RXDATAV) ==0);

	read_data = I2C0->RXDATA;

    I2C0->CMD |= I2C_CMD_ACK;

	I2C0->CMD |= I2C_CMD_STOP;

	for(int i=0;i<1000;i++);

	return read_data;
}

void I2C0_writeByte(uint8_t slave_addr, uint8_t reg_addr, uint8_t data)
{
	I2C0->TXDATA = slave_addr << 1;

	I2C0->CMD = I2C_CMD_START; //start

	while ((I2C0->IF & I2C_IF_ACK) == 0);	//wait for response

	I2C0->IFC = I2C_IFC_ACK;			//clear ACK

	I2C0->TXDATA = reg_addr;

	while ((I2C0->IF & I2C_IF_ACK) == 0);

	I2C0->IFC = I2C_IFC_ACK;

	I2C0->TXDATA = data;

	while ((I2C0->IF & I2C_IF_ACK) == 0);

	I2C0->IFC = I2C_IFC_ACK;

	I2C0->CMD = I2C_CMD_STOP;			//stop
}

void lum_enable()
{
	I2C_Enable(I2C0,true);
	I2C0_writeByte(SLAVE_ADDRESS_APDS9301, APDS9301_Command_register | APDS9301_Register_Control,0x02); //setting gain =0 and time=2 ie 402ms
	I2C0_writeByte(SLAVE_ADDRESS_APDS9301, APDS9301_Command_register | APDS9301_Register_Control,APDS9301_Control_Power_Up);//power up
}

double getLuminosityValue()
{
	uint8_t DataLow0, DataLow1, DataHigh0, DataHigh1;
	uint32_t Ch0, Ch1;
	double ratio, lux = 0;

	DataLow0 = I2C0_readByte(SLAVE_ADDRESS_APDS9301,APDS9301_Command_register | APDS9301_Word_register | APDS9301_CH0_Low);
	DataHigh0 = I2C0_readByte(SLAVE_ADDRESS_APDS9301,APDS9301_Command_register | APDS9301_Word_register | APDS9301_CH0_High);
	Ch0 = 256 * DataHigh0 + DataLow0;

	DataLow1 = I2C0_readByte(SLAVE_ADDRESS_APDS9301,APDS9301_Command_register | APDS9301_Word_register | APDS9301_CH1_Low);
	DataHigh1 = I2C0_readByte(SLAVE_ADDRESS_APDS9301,APDS9301_Command_register | APDS9301_Word_register | APDS9301_CH1_High);
	//Combining the DataHigh1 and DataLow1 into a word
	Ch1 = 256 * DataHigh1 + DataLow1;

	ratio = (double)((double)Ch1)/Ch0;
		Ch0 = Ch0*16;
		Ch1 = Ch1*16;

	if (ratio > 0 && ratio <= 0.50)
	{
		lux = 0.0304*Ch0 - 0.062*Ch0*(pow(ratio, 1.4));
	}
	else if (ratio > 0.50 && ratio <= 0.61)
	{
		lux = 0.0224*Ch0 - 0.031*Ch1;
	}
	else if (ratio > 0.61 && ratio <= 0.80)
	{
		lux = 0.0128*Ch0 - 0.0153*Ch1;
	}
	else if (ratio > 0.80 && ratio <= 1.30)
	{
		lux = 0.00146*Ch0 - 0.00112*Ch1;
	}
	else if (ratio > 1.30)
	{
		lux = 0;
	}

	return lux;
}
