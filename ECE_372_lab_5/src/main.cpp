#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"
#include "timer.h"

 #define SLA 0x68  // MPU_6050 address with PIN AD0 grounded
 #define PWR_MGMT  0x6B
 #define WAKEUP 0x00
#define SL_MEMA_XAX_HIGH  0x3B
#define SL_MEMA_XAX_LOW   0x3C
#define SL_MEMA_YAX_HIGH  0x3D
#define SL_MEMA_YAX_LOW   0x3E
#define SL_MEMA_ZAX_HIGH  0x3F
#define SL_MEMA_ZAX_LOW   0x40
#define SL_TEMP_HIGH      0x41
#define SL_TEMP_LOW       0x42





int main() {

Serial.begin(9600); // using serial port to print values from I2C bus
sei(); 
initI2C();  // initialize I2C and set bit rate

signed int X_val = 0;
signed int Y_val = 0;
signed int Z_val = 0;
//char status;

StartI2C_Trans(SLA);

//status = TWSR & 0xF8;

Write(PWR_MGMT);// address on SLA for Power Management
Write(WAKEUP); // send data to Wake up from sleep mode


//status = TWSR & 0xF8;


//



	
while (1) {
 delayMs(1000);
  
  
  Read_from(SLA,SL_MEMA_XAX_HIGH);
  X_val = Read_data(); //read upper x value
  Read_from(SLA,SL_MEMA_YAX_HIGH);
  Y_val = Read_data(); //read upper y value
  Read_from(SLA,SL_MEMA_ZAX_HIGH);
  Z_val = Read_data(); //read upper z value
  
 // status = TWSR & 0xF8;
 //Combine registers
  Serial.println("X value: "+X_val+"   Y value: "+Y_val+"   Z value: "+Z_val);
  
  StopI2C_Trans();
}

return 0;

}
