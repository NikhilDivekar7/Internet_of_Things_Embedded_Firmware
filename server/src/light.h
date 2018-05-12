
#ifndef I2C
#define I2C

#include "em_i2c.h"
#include "em_cmu.h"


#define SLAVE_ADDRESS_APDS9301         (0x39)

#define APDS9301_Command_register      (0x80)
#define APDS9301_Word_register         (0x20)
#define APDS9301_Control_Power_Up      (0x03)
#define APDS9301_Control_Power_Down    (0x00)
#define APDS9301_Register_Control      (0x00)

#define APDS9301_CH0_Low               (0x0c)
#define APDS9301_CH0_High              (0x0d)
#define APDS9301_CH1_Low               (0x0e)
#define APDS9301_CH1_High              (0x0f)

void I2C0_init(void);
uint8_t I2C0_readByte(uint8_t slave_addr,uint8_t reg_addr);
void I2C0_writeByte(uint8_t slave_addr,uint8_t reg_addr,uint8_t data);
double getLuminosityValue();
void lum_enable();


#endif
