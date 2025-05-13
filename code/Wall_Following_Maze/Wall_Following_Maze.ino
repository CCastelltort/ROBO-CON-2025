/*
  Wall Following Robot
  ---------------------
  Uses an ultrasonic sensor and a servo to detect the wall distance, then follows the left wall by adjusting direction using a two-motor system.
*/

#include <Servo.h>

// Motor Control Pins
int pinLB = 5;  // Left Motor Backward
int pinLF = 7;  // Left Motor Forward
int ena = 6;    // PWM for Left Motor

int pinRB = 2;  // Right Motor Backward
int pinRF = 4;  // Right Motor Forward
int enb = 3;    // PWM for Right Motor

// Ultrasonic Sensor Pins
int inputPin = 9;  // Define the ultrasound signal receiving a Pin (Echo)
int outputPin =8;  // Define the ultrasound signal emission Pin (Trigger)

// Constants for Wall Following
#define DESIRED_LEFT_DISTANCE 20  // Desired distance from the left wall in cm
#define TOO_CLOSE 10              // Minimum safe distance (too close)
#define TOO_FAR 30                // Maximum safe distance (too far)
#define TOLERANCE 5               // Allowable tolerance in distance measurement (in cm)

// Direction Flags
int direction = 0;
int Fgo = 8;
int Rgo = 6;
int Lgo = 4;
int Bgo = 2;

// Global Distance Variables
float Fspeedd = 0;
float Lspeedd = 0;
float Rspeedd = 0;

// Servo Setup
Servo myservo;        // Set up the myservo
int delay_time = 250; // After the servo motor to the stability of the time

// Tracking previous direction
int previous_d = 0;

void setup() {
  Serial.begin(9600);     // Initialize

  // Motor pins
  pinMode(pinLB, OUTPUT);
  pinMode(pinLF, OUTPUT);
  pinMode(pinRB, OUTPUT);
  pinMode(pinRF, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
 
 // Ultrasonic sensor pins
  pinMode(inputPin, INPUT);    // Define the ultrasound enter pin
  pinMode(outputPin, OUTPUT);  // Define the ultrasound output pin  

   // Attach servo
  myservo.attach(11);
 }

 // Stop the robot for f * 100 ms
 void stop(int f) {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,LOW);
     delay(f * 100);
    }

// Move forward for 'a' steps  
void advance(int a) {    
  for (int i = 0; i < a; i++) {
    digitalWrite(pinRB, HIGH);  
    digitalWrite(pinRF, LOW);
    digitalWrite(pinLB, HIGH);  
    digitalWrite(pinLF, LOW);
   
    analogWrite(ena, 105);      // Set left motor speed
    analogWrite(enb, 95);       // Set right motor speed
   
    delay(1000);                // Move forward for 1 second
    stop(10);                  // Stop for 1 second
  }
}
    
// Quick forward burst
void advance2(int a) {   
    digitalWrite(pinRB, HIGH);  
    digitalWrite(pinRF, LOW);
    digitalWrite(pinLB, HIGH);  
    digitalWrite(pinLF, LOW);
   
    analogWrite(ena, 115);      // Set left motor speed
    analogWrite(enb, 95);       // Set right motor speed
   
    delay(1000);                // Move forward for 1 second
    stop(10);                  // Stop for 1 second
 
    }

// Turn right in place
void right(int b) {
     digitalWrite(pinRB,LOW);  
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,LOW);
     analogWrite(enb, 150);
     delay(b * 100);
     analogWrite(enb, 0);
    }

// Turn left in place
void left(int c)         
    {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);  
     digitalWrite(pinLF,HIGH);
     //analogWrite(MotorLPWM,250);
     analogWrite(enb, 100);
     delay(c * 100);
     analogWrite(enb, 0);
    }

// Left pivot turn
void turnL(int d) {
     digitalWrite(pinRB,HIGH);  
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,HIGH);  

     analogWrite(enb, 140);
     analogWrite(ena, 160);
     delay(d * 50);

     analogWrite(ena, 0);
     analogWrite(enb, 0);
     digitalWrite(pinLF,LOW); 
     digitalWrite(pinRB,LOW);
    }

// Right pivot turn
void turnR(int e) {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,HIGH);  
     digitalWrite(pinLB,HIGH);  
     digitalWrite(pinLF,LOW);

     analogWrite(enb, 130);
     analogWrite(ena, 170);
     delay(e * 50);

     analogWrite(enb, 0);
     analogWrite(ena, 0);
    }    

// Move backward
void back(int g) {
     digitalWrite(pinRB,LOW);  
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,LOW);  
     digitalWrite(pinLF,HIGH);

     analogWrite(enb, 130);
     analogWrite(ena, 130);
     delay(g * 250); 

     analogWrite(enb, 0);
     analogWrite(ena, 0);
    }

// Get distance from ultrasonic sensor (in cm)
float get_distance() {
  digitalWrite(outputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);
  float duration = pulseIn(inputPin, HIGH);
  return duration * 0.0343 / 2.0;  // More accurate conversion
}

// Wall-following detection logic
void detection() {
  // Look straight ahead (center)
  myservo.write(90);  // Center position for the servo (look straight ahead)
  delay(delay_time);
  float frontDistance = get_distance();
  Fspeedd = frontDistance;

  // Look left
  myservo.write(5);  // Turn servo to 5 degrees (left)
  delay(delay_time);
  float leftDistance = get_distance();  // Measure left distance
  Lspeedd = leftDistance;

  // Look right (optional, for more advanced decisions)
  myservo.write(177);  // Turn servo to 177 degrees (right)
  delay(delay_time);
  float rightDistance = get_distance();  // Measure right distance
  Rspeedd = rightDistance;

  // Decide movement direction
  if (frontDistance < TOO_CLOSE) {  // If the front is too close, try to move around
    if (leftDistance < TOO_CLOSE) {  // If the left wall is too close
      direction = Bgo;  // BACK (to avoid collision)
      Serial.println("BACK");
    } else {
      direction = Rgo;  // Turn right to avoid collision ahead
      Serial.println("RIGHT");
    }
  }
  else if (leftDistance < DESIRED_LEFT_DISTANCE - TOLERANCE) {  // Too far from the left wall
    direction = Lgo;  // Turn left to get closer to the wall
    Serial.println("LEFT");
  } 
  else if (leftDistance > DESIRED_LEFT_DISTANCE + TOLERANCE) {  // Too close to the left wall
    direction = Rgo;  // Turn right to maintain distance
    Serial.println("RIGHT");
  } 
  else {
    direction = Fgo;  // If everything is good, move forward
    Serial.println("FORWARD");
  }
}

//Main loop
void loop() {
  detection();  // Update movement direction based on distances

  // Move based on directionn
  if (direction == Bgo) {  // Back up if needed
    back(2);
    stop(10);
    turnL(11);  // Turn left slightly after backing up
    stop(5);
    Serial.println("BACK");
  }
  else if (direction == Rgo) {  // Turn right if needed
    back(1);
    stop(5);
    turnR(6.5);  // Turn right slightly
    Serial.println("RIGHT");
  }
  else if (direction == Lgo) {  // Turn left if needed
    back(1);
    stop(5);
    turnL(6.5);  // Turn left slightly
    Serial.println("LEFT");
  }
  else if (direction == Fgo) {  // Move forward if everything is good
    if (previous_d == 0) {
      advance(1);  // Move forward
    } else {
      advance2(1);  // Another forward function (if needed)
    }
    Serial.println("FORWARD");
  }
}