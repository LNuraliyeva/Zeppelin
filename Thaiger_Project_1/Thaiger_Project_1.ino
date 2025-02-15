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

// Funktion: Motor auf volle Geschwindigkeit setzen
void full_speed(int pin1, int pin2){
  for (int i = 0; i <= 255; i += 15){
    analogWrite(pin1, 0);
    analogWrite(pin2, 255);
  }
}

// Funktion: Motor mit bestimmter Geschwindigkeit bewegen
void move(int pin1, int pin2, int current_speed, int pwm){
  analogWrite(pin1, 0);
  current_speed += pwm;
  analogWrite(pin2, current_speed);
}

// Funktion: Motor stoppen
void stop_motor(int pin1, int pin2, int current_speed){
  analogWrite(pin1, 0);
  analogWrite(pin2, 0);
}

// Alternative Stop-Funktion: Motor langsamer anhalten
void stop_motor2(int pin1, int pin2, int current_speed){
  for (int i = current_speed; i == 0; i -= 15)
      {
        analogWrite(pin2, i);
        delay(500);
      }
      digitalWrite(pin1, LOW);
}

// Setup-Funktion: Initialisierung von Pins und Modulen
void setup() {
  Serial.begin(115200);

  // Pin-Modi für Motoren festlegen
  pinMode(MOTOR_FORW_BACKW_PIN1, OUTPUT); 
  pinMode(MOTOR_FORW_BACKW_PIN2, OUTPUT);
  pinMode(MOTOR_UP_DOWN_PIN1, OUTPUT); 
  pinMode(MOTOR_UP_DOWN_PIN2, OUTPUT);
  pinMode(FRONT_LEFT_RIGHT_PIN1, OUTPUT); 
  pinMode(FRONT_LEFT_RIGHT_PIN2, OUTPUT);
  pinMode(BACK_LEFT_RIGHT_PIN1, OUTPUT); 
  pinMode(BACK_LEFT_RIGHT_PIN2, OUTPUT);

  // Motoren initial stoppen
  stop_motor(MOTOR_UP_DOWN_PIN1, MOTOR_UP_DOWN_PIN2, 255);
  stop_motor(MOTOR_UP_DOWN_PIN2, MOTOR_UP_DOWN_PIN1, 255);
  stop_motor(FRONT_LEFT_RIGHT_PIN1, FRONT_LEFT_RIGHT_PIN2, 255);
  stop_motor(FRONT_LEFT_RIGHT_PIN2, FRONT_LEFT_RIGHT_PIN1, 255);
  stop_motor(BACK_LEFT_RIGHT_PIN1, BACK_LEFT_RIGHT_PIN2, 255);
  stop_motor(BACK_LEFT_RIGHT_PIN2, BACK_LEFT_RIGHT_PIN1, 255);

  // Bluetooth initialisieren
  Dabble.begin("ESP32"); 
}

// Hauptprogramm (Loop)
void loop() {
  stop_motor(MOTOR_UP_DOWN_PIN1, MOTOR_UP_DOWN_PIN2, 255);
  stop_motor(MOTOR_UP_DOWN_PIN2, MOTOR_UP_DOWN_PIN1, 255);
  stop_motor(MOTOR_FORW_BACKW_PIN1, MOTOR_FORW_BACKW_PIN2, 255);
  stop_motor(MOTOR_FORW_BACKW_PIN2, MOTOR_FORW_BACKW_PIN1, 255);
  stop_motor(FRONT_LEFT_RIGHT_PIN1, FRONT_LEFT_RIGHT_PIN2, 255);
  stop_motor(FRONT_LEFT_RIGHT_PIN2, FRONT_LEFT_RIGHT_PIN1, 255);
  stop_motor(BACK_LEFT_RIGHT_PIN1, BACK_LEFT_RIGHT_PIN2, 255);
  stop_motor(BACK_LEFT_RIGHT_PIN2, BACK_LEFT_RIGHT_PIN1, 255);

  // Eingaben verarbeiten
  Dabble.processInput();

  // GamePad-Steuerung auslesen
  int radius = GamePad.getRadius();
  int angle = GamePad.getAngle();
  int speed = 0;
  int pwm = radius * 36;

  // Bewegungslogik basierend auf GamePad-Winkel
  if(45 <= angle && angle < 135){ // FORWARD
      move(MOTOR_FORW_BACKW_PIN1, MOTOR_FORW_BACKW_PIN2, speed, pwm);
  } 
  else if(315 <= angle || angle < 45){ // RIGHT
      move(FRONT_LEFT_RIGHT_PIN1, FRONT_LEFT_RIGHT_PIN2, speed, pwm);
      move(BACK_LEFT_RIGHT_PIN1, BACK_LEFT_RIGHT_PIN2, speed, pwm);    
  } 
  else if(135 <= angle && angle <= 225){ //LEFT
      move(FRONT_LEFT_RIGHT_PIN2, FRONT_LEFT_RIGHT_PIN1, speed, pwm);
      move(BACK_LEFT_RIGHT_PIN2, BACK_LEFT_RIGHT_PIN1, speed, pwm);    
  }
  else if( 225 < angle && angle  < 315){ //BACKWARD
      move(MOTOR_FORW_BACKW_PIN2, MOTOR_FORW_BACKW_PIN1, speed, pwm);    
  }
  else if(radius == 0){ // STOP
      stop_motor2(MOTOR_FORW_BACKW_PIN1, MOTOR_FORW_BACKW_PIN2, pwm);
      stop_motor2(MOTOR_FORW_BACKW_PIN2, MOTOR_FORW_BACKW_PIN1, pwm);
      stop_motor(MOTOR_FORW_BACKW_PIN1, MOTOR_FORW_BACKW_PIN2, pwm);
      stop_motor(MOTOR_FORW_BACKW_PIN2, MOTOR_FORW_BACKW_PIN1, pwm);
      
      if((!GamePad.isSquarePressed())){
        stop_motor(FRONT_LEFT_RIGHT_PIN1, FRONT_LEFT_RIGHT_PIN2, pwm);
        // stop_motor(FRONT_LEFT_RIGHT_PIN2, FRONT_LEFT_RIGHT_PIN1, pwm);

        stop_motor(BACK_LEFT_RIGHT_PIN2, BACK_LEFT_RIGHT_PIN1, pwm);
        // stop_motor(BACK_LEFT_RIGHT_PIN1, BACK_LEFT_RIGHT_PIN2, pwm);
      }
      if(!(GamePad.isCirclePressed())){
        // stop_motor(FRONT_LEFT_RIGHT_PIN1, FRONT_LEFT_RIGHT_PIN2, pwm);
        stop_motor(FRONT_LEFT_RIGHT_PIN2, FRONT_LEFT_RIGHT_PIN1, pwm);

        // stop_motor(BACK_LEFT_RIGHT_PIN2, BACK_LEFT_RIGHT_PIN1, pwm);
        stop_motor(BACK_LEFT_RIGHT_PIN1, BACK_LEFT_RIGHT_PIN2, pwm);
      }
  }
  //LINKS UM DIE EIGENE ACHSE
  // FRONT MOTOREN LINKS + HINTERE RECHTS
  if (GamePad.isSquarePressed()){  
    full_speed(FRONT_LEFT_RIGHT_PIN1, FRONT_LEFT_RIGHT_PIN2);
    full_speed(BACK_LEFT_RIGHT_PIN2, BACK_LEFT_RIGHT_PIN1);
  }
  //RECHTS UM DIE EIGENE ACHSE
  // FRONT MOTOREN RECHTS + HINTERE LINKS 
  if (GamePad.isCirclePressed()){
    full_speed(FRONT_LEFT_RIGHT_PIN2, FRONT_LEFT_RIGHT_PIN1);
    full_speed(BACK_LEFT_RIGHT_PIN1, BACK_LEFT_RIGHT_PIN2);
  }
  //DOWN
  if (GamePad.isCrossPressed()){ 
    full_speed(MOTOR_UP_DOWN_PIN1, MOTOR_UP_DOWN_PIN2);
    delay(500);
  }

  if (GamePad.isTrianglePressed()){ 
    full_speed(MOTOR_UP_DOWN_PIN2, MOTOR_UP_DOWN_PIN1);
    delay(500);
  }
  //UP
  if (GamePad.isSelectPressed()){
    stop_motor(MOTOR_UP_DOWN_PIN1, MOTOR_UP_DOWN_PIN2, 255);
    stop_motor(MOTOR_UP_DOWN_PIN2, MOTOR_UP_DOWN_PIN1, 255);
    stop_motor(MOTOR_FORW_BACKW_PIN1, MOTOR_FORW_BACKW_PIN2, 255);
    stop_motor(MOTOR_FORW_BACKW_PIN2, MOTOR_FORW_BACKW_PIN1, 255);
    stop_motor(FRONT_LEFT_RIGHT_PIN1, FRONT_LEFT_RIGHT_PIN2, 255);
    stop_motor(FRONT_LEFT_RIGHT_PIN2, FRONT_LEFT_RIGHT_PIN1, 255);
    stop_motor(BACK_LEFT_RIGHT_PIN1, BACK_LEFT_RIGHT_PIN2, 255);
    stop_motor(BACK_LEFT_RIGHT_PIN2, BACK_LEFT_RIGHT_PIN1, 255);
  }

  delay(500); // Wartezeit zwischen Loop-Durchläufen
}
