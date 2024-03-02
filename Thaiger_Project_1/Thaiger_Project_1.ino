#include <Arduino.h>
#include "BluetoothSerial.h"

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

#define MOTOR34_DIR_PIN 12
#define MOTOR34_ENABLE_PIN 13

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void full_speed(int pin1, int pin2, bool foreward){

  if (foreward){
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }else{
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  
  }
}

void move(int pin1, int pin2, int current_speed, int increment){

  digitalWrite(pin1, LOW);
  current_speed += increment;
  analogWrite(pin2, current_speed);
}

void stop_motor(int pin1, int pin2, int current_speed){

  for (int i = current_speed; i == 0; i -= 15)
      {
        analogWrite(pin2, i);
        delay(500);
      }

      digitalWrite(pin1, LOW);

}

void setup() {

  Serial.begin(115200);

  pinMode(MOTOR34_DIR_PIN, OUTPUT); 
  pinMode(MOTOR34_ENABLE_PIN, OUTPUT);


  Dabble.begin("ESP32"); 

}



void loop() {

  Dabble.processInput();

  int radius = GamePad.getRadius();
  int angle = GamePad.getAngle();

  int ourConst = radius * 36;
  int speed = 0;
  //int pwm = ourConst;
  
  if(45 <= angle && angle < 135){ // UP

      move_forward(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, speed, ourConst);
      
      //move(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, pwm);

  // } else if( (315 <= angle || angle < 45) && angle != 0){ // RIGHT

  //     stop_motor(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, speed);
  
  // } else if(135 <= angle && angle <= 225){ //LEFT

  //     stop_motor(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, speed);    
      
  }else if( 225 < angle && angle  < 315){ //DOWN

      move_forward(MOTOR34_ENABLE_PIN, MOTOR34_DIR_PIN, speed, ourConst);
      

  }else if(radius == 0){ // STOP

      stop_motor(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, speed);
      stop_motor(MOTOR34_ENABLE_PIN, MOTOR34_DIR_PIN, speed);
       
  }


  if (GamePad.isSquarePressed())
  {

    Serial.print("Square");
  }

  if (GamePad.isCirclePressed())
  {
    Serial.print("Circle");
  }

  if (GamePad.isCrossPressed())
  {
    set_low(MOTOR34_ENABLE_PIN, MOTOR34_DIR_PIN);
  
  }

  if (GamePad.isTrianglePressed())
  {
    set_high(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN);
  
  }

  delay(500);
}