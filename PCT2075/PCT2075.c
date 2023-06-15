/*
 * PCT2075.c
 *
 *  Created on: 01 Jun 2023
 *      Author: Tevin Marindi
 *      @brief: HAL driver for the PCT2075 temperature sensor
 *
 *      "A Poul-Henning Kamp THE BEER-WARE LICENSE" (Revision 42): As long as you retain this notice you
 * 		can do whatever you want with this stuff. If we meet some day, and you think
 * 		this stuff is worth it, you can buy me a beer in return.
 *
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "PCT2075.h"

PCT2075_typedef *pct2075;

/* Initialize and set up the communication physical layer (I2C)
 * for the PCT2075 ambient temperature sensor.
 */
void PCT2075_init(PCT2075_typedef *driver)
{
	pct2075=driver;
}


/*write to PCT2075 register */

uint8_t PCT2075_set_register(pct2075_data pdata[],uint8_t len)
{
	uint8_t results = 0;
	uint8_t write_address = (pct2075->i2c_addrs << 1ul);

	results = pct2075->i2c_HAL_Master_TX(pct2075->i2c_port, write_address,pdata, len, 100);
	return results;

}

/*Read from PCT2075 register */
uint8_t PCT2075_get_register(PCT2075_Register pRegister,pct2075_data *pdata,pct2075_data_len len)
{
	PCT2075_Register pct2075_register=pRegister;
	uint8_t results = 0;
	uint8_t read_address = (pct2075->i2c_addrs << 1ul) | 1ul;
	uint8_t write_address = (pct2075->i2c_addrs << 1ul);

	/*Send pointer value*/
	results=pct2075->i2c_HAL_Master_TX(pct2075->i2c_port, write_address, &pct2075_register,1, 100);

	/*Send register value*/
	if(results==PCT2075_I2C_TRANSMIT_SUCCESSFUL)
	{
		results = pct2075->i2c_HAL_Master_RX(pct2075->i2c_port, read_address, pdata,len, 100);
	}
	return results;
}

/*Get The temperature value from any temperature related register*/
uint8_t PCT2075_get_temp_register(PCT2075_Register pRegister,ptr_pct2075_temperature *value)
{
	uint8_t results = 0;
	uint8_t pdata[2];
	uint16_t rawtemperature_data = 0;

	results = PCT2075_get_register(pRegister, pdata, 2);

	/*Check if reading data from the temp register
	 *else process the data as if reading from Tos or Thyst register*/

	if (results == PCT2075_I2C_RECEIVE_SUCCESSFUL) {
		if(pRegister==PCT2075_REGISTER_TEMP){
			rawtemperature_data |= (uint16_t) (pdata[0] << 3);
			rawtemperature_data |= pdata[1] >> 5;

			/*Check if the Temperature value is a negative value
			 * If the Temp data MSByte bit D10 = 1, then the temperature is negative and Temp
			 * value (°C) = -(two’s complement of Temp data) × 0.125 °C.
			 */
			if (rawtemperature_data & Negative_temperature_bit_Msk) {
				rawtemperature_data = ~rawtemperature_data;
				rawtemperature_data = rawtemperature_data - 63488;
				rawtemperature_data += 1;
				*value = (ptr_pct2075_temperature) (rawtemperature_data * 0.125)* -1;

			} else {
				*value =(ptr_pct2075_temperature) (rawtemperature_data * 0.125);
			}
		}else{
			rawtemperature_data |= (uint16_t) (pdata[0] << 1);
			rawtemperature_data |= pdata[1] >> 7;

			if (rawtemperature_data & Negative_temperature_tos_bit_Msk) {
				rawtemperature_data = ~rawtemperature_data;
				rawtemperature_data = rawtemperature_data - 65024;
				rawtemperature_data += 1;
				*value = (ptr_pct2075_temperature) (rawtemperature_data /2)* -1;

			} else {
				*value =(ptr_pct2075_temperature) rawtemperature_data/2;
			}
		}
	}

	return results;
}

/*Get The temperature value*/
uint8_t PCT2075_get_temperature(ptr_pct2075_temperature *value)
{
	return PCT2075_get_temp_register(PCT2075_REGISTER_TEMP,value);
}

/*Get the High temperature threshold*/
uint8_t PCT2075_get_high_temp_threshold(ptr_pct2075_temperature *value)
{
	return PCT2075_get_temp_register(PCT2075_REGISTER_TOS,value);
}

/*Get the  Hysteresis Temperature value*/
uint8_t PCT2075_get_hysteresis(ptr_pct2075_temperature *value)
{
	return PCT2075_get_temp_register(PCT2075_REGISTER_THYST,value);
}

/*Set the High temperature threshold*/
uint8_t PCT2075_set_high_temp_threshold(ptr_pct2075_temperature value)
{
	uint8_t txdata[3];

	txdata[0]=PCT2075_REGISTER_TOS;
	txdata[1]=(uint8_t)value;

	if(value-(uint8_t)value==0.5){
		txdata[2]=0x80;
	}else{
		txdata[2]=0;
	}

	uint8_t results=PCT2075_set_register(txdata,3);

	return results;
}

/*Set Hysteresis temp value the lazy way*/
uint8_t PCT2075_set_hysteresis(ptr_pct2075_temperature value)
{
	uint8_t txdata[3];

	txdata[0]=PCT2075_REGISTER_THYST;
	txdata[1]=(uint8_t)value;

	if(value-(uint8_t)value==0.5){
		txdata[2]=0x80;
	}else{
		txdata[2]=0;
	}

	uint8_t results=PCT2075_set_register(txdata,3);
	return results;
}

uint8_t PCT2075_set_tidle(pct2075_data value)
{
	uint8_t results = 0;
	uint8_t txdata[2];

	txdata[0]=PCT2075_REGISTER_TIDLE;
	txdata[1]=(uint8_t)value;

	if (value < 0x20) {
		results = PCT2075_set_register(txdata, 2);
	} else {
		return false;
	}
	return results;
}

/*get Tidle*/
uint8_t PCT2075_get_tidle(pct2075_data *data)
{
	return PCT2075_get_register(PCT2075_REGISTER_TIDLE, data, 1);
}

/*Write to the config register*/
uint8_t PCT2075_update_config_register(PCT2075_CONFIG config)
{
	uint8_t conf_data=config.deviceOperationMode
			+config.operationMode
			+config.outputPolarity
			+config.faultQueue;

	uint8_t txdata[2];
		txdata[0]=PCT2075_REGISTER_CONF;
		txdata[1]=conf_data;

	 PCT2075_set_register(txdata, 2);

	return true;
}

