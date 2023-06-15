# pct2075
USAGE : 

int main(void)
{
  uint8_t i2c_status;
  PCT2075_typedef pct2075;
	ptr_pct2075_temperature temperature_value;

  
  pct2075.i2c_port=I2C1;
	pct2075.i2c_addrs=PCT2075_I2C_DEVICE_ADDRESS;
	pct2075.i2c_HAL_Master_TX=i2c_masterTransmit; // or STM32 HAL masterTransmit
	pct2075.i2c_HAL_Master_RX=i2c_masterReceive;  // or STM32 HAL i2c_masterReceive
  
  PCT2075_init(&pct2075);
  
  i2c_status=PCT2075_get_temperature(&temperature_value);
  
  if(i2c_status==true){
		    	printf("Ambient temperature %fC , I2C status %d\n\r ",temperature_value,i2c_status);
    }
  
  
  

}
