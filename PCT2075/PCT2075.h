/*
 * PCT2075.h
 *
 *  Created on: 01 Jun 2023
 *      Author: Tevin Marindi
 *      @brief: HAL driver for the PCT2075 temperature sensor
 *
 *      "A Poul-Henning Kamp THE BEER-WARE LICENSE" (Revision 42): As long as you retain this notice you
 * 		can do whatever you want with this stuff. If we meet some day, and you think
 * 		this stuff is worth it, you can buy me a beer in return.
 */

#ifndef PCT2075_PCT2075_H_
#define PCT2075_PCT2075_H_

#include <main.h>

#define PCT2075_I2C_DEVICE_ADDRESS			0x77		//1110111

#define PCT2075_I2C_TRANSMIT_SUCCESSFUL		0x8ul		//Change this to match the HAL successful transmit status
#define PCT2075_I2C_RECEIVE_SUCCESSFUL		0x1ul		//Change this to match the HAL successful receive status

#define PCT2075_REGISTER_TEMP				0x0ul		//Temperature register
#define PCT2075_REGISTER_CONF				0x1ul		//Configuration register
#define	PCT2075_REGISTER_THYST				0x2ul		//Hysteresis register
#define PCT2075_REGISTER_TOS				0x3ul		//Overtemperature shutdown threshold register
#define PCT2075_REGISTER_TIDLE				0x4ul		//Temperature conversion cycle

#define Negative_temperature_bit_Pos		0xAul		//0b10000000000
#define Negative_temperature_bit_Msk		(0x1uL << Negative_temperature_bit_Pos)

#define Negative_temperature_tos_bit_Pos	0x8ul		//0b100000000
#define Negative_temperature_tos_bit_Msk	(0x1uL << Negative_temperature_tos_bit_Pos)


typedef uint8_t PCT2075_Register;
typedef uint8_t pct2075_device_address;
typedef uint8_t pct2075_data;
typedef uint16_t pct2075_data_len;
typedef uint32_t pct2075_i2c_timeout;
typedef I2C_TypeDef	pct2075_i2c_port;
typedef float ptr_pct2075_temperature;

/*
 * PCT2075 Temperature sensor structure to be registered from main.c file
 */
typedef struct _PCT2075_typedef
{

	pct2075_i2c_port *i2c_port;				/*I2C port*/
	pct2075_device_address	i2c_addrs;		/*7 bit I2C device address without the read or write lSB */

	/*Function pointer to HAL_I2C_Master_Transmit() function to be used by the temperature sensor*/
	uint8_t (*i2c_HAL_Master_TX)(pct2075_i2c_port *i2c_port,pct2075_device_address i2c_addrs,pct2075_data *pdata,
			pct2075_data_len len,pct2075_i2c_timeout timeout);

	/*Function pointer to HAL_I2C_Master_Receive() function to be used by the temperature sensor*/
	uint8_t (*i2c_HAL_Master_RX)(pct2075_i2c_port *i2c_port,pct2075_device_address i2c_addrs,pct2075_data *pdata,
			pct2075_data_len len,pct2075_i2c_timeout timeout);

}PCT2075_typedef;

/*
 * @Brief :Fault queue is defined as the number of faults
 *  that must occur consecutively to activate the OS (OUTPUT STATE) output.
 */
typedef enum {
	OS_FAULT_QUEUE_01,
	OS_FAULT_QUEUE_02,
	OS_FAULT_QUEUE_04,
	OS_FAULT_QUEUE_06
}PCT2075_OS_FAULT_QUEUE;

typedef enum{
	POl_ACTIVE_LOW,
	POl_ACTIVE_HIGH
}PCT2075_OS_OUTPUT_POLARITY;

typedef enum{
	COMPARATOR_MODE,
	INTERRUPT_MODE
}PCT2075_OS_OPERATION_MODE;

typedef enum{
	NORMAL_OPERATION ,
	SHUTDOWN_OPERATION
}PCT2075_DEVICE_OPERATION_MODE;


typedef struct _PCT2075_CONFIG
{
    PCT2075_OS_FAULT_QUEUE faultQueue;
    PCT2075_OS_OUTPUT_POLARITY outputPolarity;
    PCT2075_OS_OPERATION_MODE operationMode;
    PCT2075_DEVICE_OPERATION_MODE deviceOperationMode;
}PCT2075_CONFIG;


/* Initialize and set up the communication physical layer (I2C)
 * for the PCT2075 ambient temperature sensor.
 */
void PCT2075_init(PCT2075_typedef *driver);


/*Write to PCT2075 register */
uint8_t PCT2075_set_register(pct2075_data *pdata,uint8_t len);

/*Read from PCT2075 register */
uint8_t PCT2075_get_register(PCT2075_Register pRegister,pct2075_data *pdata,pct2075_data_len len);

/*Get The temperature value from any temperature related register*/
uint8_t PCT2075_get_temp_register(PCT2075_Register pRegister,ptr_pct2075_temperature *value);

/*Get The temperature value*/
uint8_t PCT2075_get_temperature(ptr_pct2075_temperature *value);

/*Get the High temperature threshold*/
uint8_t PCT2075_get_high_temp_threshold(ptr_pct2075_temperature *value);

/*Get the  Hysteresis Temperature value*/
uint8_t PCT2075_get_hysteresis(ptr_pct2075_temperature *value);

/*Set the High temperature threshold*/
uint8_t PCT2075_set_high_temp_threshold(ptr_pct2075_temperature value);

/*Set Hysteresis temp value the lazy way*/
uint8_t PCT2075_set_hysteresis(ptr_pct2075_temperature value);

/*Set Tidle*/
uint8_t PCT2075_set_tidle(pct2075_data value);

/*get Tidle*/
uint8_t PCT2075_get_tidle(pct2075_data *data);

/*Write to the config register*/
uint8_t PCT2075_update_config_register(PCT2075_CONFIG config);

#endif /* PCT2075_PCT2075_H_ */
