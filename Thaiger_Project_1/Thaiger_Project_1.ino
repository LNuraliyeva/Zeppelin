#include <Arduino.h>
#include "BluetoothSerial.h"

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

#define MOTOR34_DIR_PIN 35
#define MOTOR34_ENABLE_PIN 26

// TEst 




#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void set_high(int dirPin, int enPin){

  digitalWrite(dirPin, HIGH);
  analogWrite(enPin, 200);
}

void set_low(int dirPin, int enPin){

  digitalWrite(dirPin, HIGH);
  analogWrite(enPin, 200);
}

void move_forward(int dirPin, int enPin, int current_speed, int increment){

  digitalWrite(dirPin, LOW);
  current_speed += increment;
  analogWrite(enPin, current_speed);
}

void move (int pin1, int pin2, int pwm){
  analogWrite(pin1, 0);
  analogWrite(pin2, pwm);
}

void stop_motor(int dirPin, int enPin, int current_speed){

  for (int i = current_speed; i == 0; i -= 15)
      {
        analogWrite(enPin, i);
        delay(500);
      }

      digitalWrite(dirPin, LOW);

}

// void move_backward(int dirPin, int enPin, int current_speed, int increment){

//   digitalWrite(dirPin, LOW);
//   current_speed += increment;
//   analogWrite(enPin, current_speed);
// }

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
  int pwm = ourConst;
  
  if(45 <= angle && angle < 135){ // UP

      //move_forward(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, speed, ourConst);
      //move_forward(MOTOR34_ENABLE_PIN, MOTOR34_DIR_PIN, speed, ourConst);
      move(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, pwm);

  // } else if( (315 <= angle || angle < 45) && angle != 0){ // RIGHT

  //     stop_motor(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, speed);
  
  // } else if(135 <= angle && angle <= 225){ //LEFT

  //     stop_motor(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, speed);    
      
  }else if( 225 < angle && angle  < 315){ //DOWN

      //move_forward(MOTOR34_ENABLE_PIN, MOTOR34_DIR_PIN, speed, ourConst);
      //move(MOTOR34_ENABLE_PIN, MOTOR34_DIR_PIN, pwm);
      Serial.print("Pulled Down");

  }else if(radius == 0){ // STOP

      stop_motor(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN, speed);
      
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
    set_low(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN);
  
  }

  if (GamePad.isTrianglePressed())
  {
    set_high(MOTOR34_DIR_PIN, MOTOR34_ENABLE_PIN);
  
  }

  delay(500);
}