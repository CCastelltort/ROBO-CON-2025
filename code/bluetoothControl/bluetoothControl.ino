/*
  bluetoothControl.ino
  ----------------------
  Remote control code for Arduino Uno robot using HM-10 Bluetooth module and Dabble app.

  Features:
  - Forward, Backward, Left, Right controls via Dabble Gamepad
  - PWM motor speed control
  - Debug messages via Serial Monitor
*/

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
#include <SoftwareSerial.h>

//Define motor pins
int pinLB = A1; //Left Back Motor
int pinLF = A0; //Left Forward Motor
int pinRB = A3; //Right Back Motor
int pinRF = A2; //Right Forward Motor
int ena = 5; //PWM control for left motor
int enb = 3; //PWM control for right motor

//Define SoftwareSerial pins (RX, TX)
SoftwareSerial BTSerial(7, 6); //RX, TX (using pins 7 and 6 for Bluetooth)


void setup() {
  Serial.begin(9600);      //Start the hardware serial for debugging
  BTSerial.begin(9600);    //Initialize Bluetooth communication on SoftwareSerial pins 6 and 7
  Dabble.begin(BTSerial);  //Initialize Dabble with SoftwareSerial

  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
}

void loop() {
  Dabble.processInput(); //Read inputs from the Dabble app

  Serial.print("KeyPressed: ");
  if (GamePad.isUpPressed()) {
    Serial.println("UP");
    moveForward();
  } else if (GamePad.isDownPressed()) {
    Serial.println("DOWN");
    moveBackward();
  } else if (GamePad.isLeftPressed()) {
    Serial.println("LEFT");
    turnLeft();
  } else if (GamePad.isRightPressed()) {
    Serial.println("RIGHT");
    turnRight();
  } else {
    stopMotors();
  }
}

//Move Forward (left back and right back motors high)
void moveForward() {
  digitalWrite(pinLF, LOW);   
  digitalWrite(pinLB, HIGH);  
  digitalWrite(pinRF, LOW);  
  digitalWrite(pinRB, HIGH);

  analogWrite(ena, 255); //Set left motor speed
  analogWrite(enb, 205); //Set right motor speed
}

//Move Backward (left front and right front motors high)
void moveBackward() {
  digitalWrite(pinLF, HIGH);  
  digitalWrite(pinLB, LOW);  
  digitalWrite(pinRF, HIGH); 
  digitalWrite(pinRB, LOW); 
}

//Turn left (left front and right back motors high)
void turnLeft() {
  digitalWrite(pinLF, HIGH);  
  digitalWrite(pinLB, LOW);  
  digitalWrite(pinRF, LOW);  
  digitalWrite(pinRB, HIGH);
}

//Turn right (left back and right front motors high)
void turnRight() {
  digitalWrite(pinLF, LOW);  
  digitalWrite(pinLB, HIGH); 
  digitalWrite(pinRF, HIGH); 
  digitalWrite(pinRB, LOW);  
}

void stopMotors() {
  digitalWrite(pinLF, LOW);   // Stop Left Motor
  digitalWrite(pinLB, LOW);   // Stop Left Motor
  digitalWrite(pinRF, LOW);   // Stop Right Motor
  digitalWrite(pinRB, LOW);   // Stop Right Motor
}
