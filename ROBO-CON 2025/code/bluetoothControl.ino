#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
#include <SoftwareSerial.h>

// Define motor pins
int pinLB = A1;  // Left Back Motor
int pinLF = A0;  // Left Forward Motor
int pinRB = A3;  // Right Back Motor
int pinRF = A2;  // Right Forward Motor
int ena = 5;     // PWM control for left motor
int enb = 3;     // PWM control for right motor

// Define SoftwareSerial pins (RX, TX)
SoftwareSerial BTSerial(7, 6); // RX, TX (using pins 7 and 6 for Bluetooth)

void setup() {
  Serial.begin(9600);      // Start the hardware serial for debugging
  BTSerial.begin(9600);    // Initialize Bluetooth communication on SoftwareSerial pins 6 and 7
  Dabble.begin(BTSerial);  // Initialize Dabble with SoftwareSerial

  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
}

void loop() {
  Dabble.processInput(); // This function reads inputs from the Dabble app

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

void moveForward() {
  digitalWrite(pinLF, LOW);   // Forward for Left Motor
  digitalWrite(pinLB, HIGH);  // Backward for Left Motor
  digitalWrite(pinRF, LOW);   // Forward for Right Motor
  digitalWrite(pinRB, HIGH);  // Backward for Right Motor

  analogWrite(ena, 255);      // Set left motor speed
  analogWrite(enb, 205);       // Set right motor speed
}

void moveBackward() {
  digitalWrite(pinLF, HIGH);  // Backward for Left Motor
  digitalWrite(pinLB, LOW);   // Forward for Left Motor
  digitalWrite(pinRF, HIGH);  // Backward for Right Motor
  digitalWrite(pinRB, LOW);   // Forward for Right Motor
}

void turnLeft() {
  digitalWrite(pinLF, HIGH);  // Backward for Left Motor
  digitalWrite(pinLB, LOW);   // Forward for Left Motor
  digitalWrite(pinRF, LOW);   // Forward for Right Motor
  digitalWrite(pinRB, HIGH);  // Backward for Right Motor
}

void turnRight() {
  digitalWrite(pinLF, LOW);   // Forward for Left Motor
  digitalWrite(pinLB, HIGH);  // Backward for Left Motor
  digitalWrite(pinRF, HIGH);  // Backward for Right Motor
  digitalWrite(pinRB, LOW);   // Forward for Right Motor
}

void stopMotors() {
  digitalWrite(pinLF, LOW);   // Stop Left Motor
  digitalWrite(pinLB, LOW);   // Stop Left Motor
  digitalWrite(pinRF, LOW);   // Stop Right Motor
  digitalWrite(pinRB, LOW);   // Stop Right Motor
}
