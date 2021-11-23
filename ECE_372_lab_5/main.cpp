
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer.h"
#include "pwm.h"
#include "switch.h"
#include "I2C.h"
#include "math.h"
#include "spi.h"
#define SLA 0x68  // MPU_6050 address with PIN AD0 grounded
#define PWR_MGMT  0x6C
#define WAKEUP 0x00
#define SL_MEMA_XAX_HIGH  0x3B
#define SL_MEMA_XAX_LOW   0x3C
#define SL_MEMA_YAX_HIGH  0x3D
#define SL_MEMA_YAX_LOW   0x3E
#define SL_MEMA_ZAX_HIGH  0x3F
#define SL_MEMA_ZAX_LOW   0x40
#define SL_TEMP_HIGH      0x41   // (this address only for testing)
#define SL_TEMP_LOW       0x42  // (this address only for testing)

typedef enum stateEnumaration{wait_press, debounce_press, wait_release, debounce_release}state;  // initial enum for states
volatile state button_state = wait_press; 
volatile int alarm_state = 1;  // not alarming state
int main() {
Serial.begin(9600); // using serial port to print values from I2C bus
sei(); 
initI2C();  // initialize I2C and set bit rate
initTimerMs();
initSwitchPD0();
initSPI();

signed int X_val = 0;
signed int Y_val = 0;
signed int Z_val = 0;
float tilt_x = 0.0;
float tilt_y = 0.0;
StartI2C_Trans(SLA);
write(PWR_MGMT);// address on SLA for Power Management
write(WAKEUP); // send data to Wake up from sleep mode
StopI2C_Trans();
initPWM();



while (1) {
  
  Read_from(SLA,SL_MEMA_XAX_HIGH );
  X_val= Read_data(); // read upper value
  Read_from(SLA,SL_MEMA_XAX_LOW);
  X_val = (X_val << 8 )| Read_data(); // append lower value
  StopI2C_Trans();

  Read_from(SLA,SL_MEMA_YAX_HIGH );
  Y_val= Read_data(); // read upper value
  Read_from(SLA,SL_MEMA_YAX_LOW);
  Y_val = (Y_val << 8 )| Read_data(); // append lower value
  StopI2C_Trans();

  Read_from(SLA,SL_MEMA_ZAX_HIGH );
  Z_val= Read_data(); // read upper value
  Read_from(SLA,SL_MEMA_ZAX_LOW);
  Z_val = (Z_val << 8 )| Read_data(); // append lower value
  StopI2C_Trans();
  
 tilt_x = float(X_val);
 tilt_y = float(Y_val);
 delayMs(500);
  if((tilt_x > 8000) | (tilt_x < -8000) | (tilt_y > 8000) | (tilt_y < -8000)){      // tilt more than 45 degree
      if(alarm_state == 2){  // button pressed 
     // Serial.println("Button Pressed");
      OCR4A = 0;
      }else{
      alarm(500);
      delayMs(100);
      alarm(1000);
      delayMs(100);
      alarm(1500);
      delayMs(100);
      alarm(2000);
      delayMs(100);
      alarm(2500);
      delayMs(100);
      OCR4A = 0;
        // Serial.println("Not Pressed");
            // ======SPI ============
        display(0x01,0b00011110);
        display(0x02,0b01000001);
        display(0x03,0b00100100);
        display(0x04,0b00010000);
        display(0x05,0b00010000);
        display(0x06,0b00100100);
        display(0x07,0b01000001);
        display(0x08,0b00011110);

      // ========SPI end ========= 
      }
     

   ///*
    Serial.println("        ");
    Serial.println("Alarm State ");
    Serial.print("X-val: ");
    Serial.println(X_val);
    Serial.print("Y-val: ");
    Serial.println(Y_val);
    Serial.print("Z-val: ");
    Serial.println(Z_val);
    // */

  }else{
    OCR4A = 0;
    Serial.println("Normal Mode");
   ///*
    Serial.println("        ");
    Serial.println("Normal Mode");
    Serial.print("X-val: ");
    Serial.println(X_val);
    Serial.print("Y-val: ");
    Serial.println(Y_val);
    Serial.print("Z-val: ");
    Serial.println(Z_val);
    //*/

    // ======SPI ============
      display(0x01,0b00011110);
      display(0x02,0b00100001);
      display(0x03,0b01000100);
      display(0x04,0b10000000);
      display(0x05,0b10000000);
      display(0x06,0b01000100);
      display(0x07,0b00100001);
      display(0x08,0b00011110);

    // ========SPI end ========= 
  }
  Serial.flush();

  // ===============state machine =====================// 
       switch (button_state){
              case wait_press:
              break;
              case debounce_press:
              delayMs(1); // delay for 1 milisecond
              button_state = wait_release;
              break;
              case wait_release:
              //button_state = debounce_press;
              break;
              case debounce_release:
              delayMs(1);
              button_state = wait_press;
              break;
                          
        }
  // =======================state machine end ===========// 
}

return 0;
}

ISR(INT2_vect){ //pin change interrrupt service register
        if (button_state == wait_press){
          button_state = debounce_press;  // change to debounce_press state
        }else if(button_state == wait_release){
          if(alarm_state == 1){  // check for the tutton state
            alarm_state = 2;
    
          }
          else if(alarm_state == 2){
            alarm_state = 1;
          }
          button_state = debounce_release;  // change to debounce_release state
        }
}

