#include <Arduino.h>
#include "BluetoothSerial.h"

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

#define MOTOR_FORW_BACKW_PIN1 12
#define MOTOR_FORW_BACKW_PIN2 13
#define MOTOR_UP_DOWN_PIN1 14
#define MOTOR_UP_DOWN_PIN2 27
#define FRONT_LEFT_RIGHT_PIN1 26
#define FRONT_LEFT_RIGHT_PIN2 25
#define BACK_LEFT_RIGHT_PIN1 33
#define BACK_LEFT_RIGHT_PIN2 32

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


void full_speed(int pin1, int pin2){
  for (int i = 0; i <= 255; i += 15){
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  }
}


void move(int pin1, int pin2, int current_speed, int pwm){
  digitalWrite(pin1, LOW);
  current_speed += pwm;
  analogWrite(pin2, current_speed);
  analogWrite(pin2, pwm);
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

  pinMode(MOTOR_FORW_BACKW_PIN1, OUTPUT); 
  pinMode(MOTOR_FORW_BACKW_PIN2, OUTPUT);
  pinMode(MOTOR_UP_DOWN_PIN1, OUTPUT); 
  pinMode(MOTOR_UP_DOWN_PIN2, OUTPUT);

  pinMode(FRONT_LEFT_RIGHT_PIN1, OUTPUT); 
  pinMode(FRONT_LEFT_RIGHT_PIN2, OUTPUT);
  pinMode(BACK_LEFT_RIGHT_PIN1, OUTPUT); 
  pinMode(BACK_LEFT_RIGHT_PIN2, OUTPUT);

  stop_motor(MOTOR_UP_DOWN_PIN1, MOTOR_UP_DOWN_PIN2, 255);
  stop_motor(MOTOR_UP_DOWN_PIN2, MOTOR_UP_DOWN_PIN1, 255);

  Dabble.begin("ESP32"); 
}


void loop() {
  stop_motor(MOTOR_UP_DOWN_PIN1, MOTOR_UP_DOWN_PIN2, 255);
  stop_motor(MOTOR_UP_DOWN_PIN2, MOTOR_UP_DOWN_PIN1, 255);
  Dabble.processInput();

  int radius = GamePad.getRadius();
  int angle = GamePad.getAngle();
  int speed = 0;
  int pwm = radius * 36;

  if(45 <= angle && angle < 135){ // FORWARD
      move(MOTOR_FORW_BACKW_PIN1, MOTOR_FORW_BACKW_PIN2, speed, pwm);
      
      //move(MOTOR_FORW_BACKW_PIN1, MOTOR_FORW_BACKW_PIN2, pwm);

  } else if(315 <= angle || angle < 45){ // RIGHT

      move(FRONT_LEFT_RIGHT_PIN1, FRONT_LEFT_RIGHT_PIN2, speed, pwm);
  
  } else if(135 <= angle && angle <= 225){ //LEFT

      move(BACK_LEFT_RIGHT_PIN1, BACK_LEFT_RIGHT_PIN2, speed, pwm);    
      
  }
  else if( 225 < angle && angle  < 315){ //BACKWARD
      move(MOTOR_FORW_BACKW_PIN2, MOTOR_FORW_BACKW_PIN1, speed, pwm);    
  }
  else if(radius == 0){ // STOP
      stop_motor(MOTOR_FORW_BACKW_PIN1, MOTOR_FORW_BACKW_PIN2, pwm);
      stop_motor(MOTOR_FORW_BACKW_PIN2, MOTOR_FORW_BACKW_PIN1, pwm);
  }


  if (GamePad.isSquarePressed()){
    Serial.print("Square");
  }

  if (GamePad.isCirclePressed()){
    Serial.print("Circle");
  }

  if (GamePad.isCrossPressed()){ //DOWN
    full_speed(MOTOR_UP_DOWN_PIN1, MOTOR_UP_DOWN_PIN2);
    delay(500);
  }

  if (GamePad.isTrianglePressed()){ //UP
    full_speed(MOTOR_UP_DOWN_PIN2, MOTOR_UP_DOWN_PIN1);
    delay(500);
  }

  if (GamePad.isSelectPressed()){
    stop_motor(MOTOR_UP_DOWN_PIN1, MOTOR_UP_DOWN_PIN2, 255);
    stop_motor(MOTOR_UP_DOWN_PIN2, MOTOR_UP_DOWN_PIN1, 255);
  }

  delay(500);
}