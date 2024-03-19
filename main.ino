#include <Arduino.h>

// Pin definitions
#define M1L 11
#define M1R 10
#define M2L 5
#define M2R 6
#define JSUMO_SWITCH 4
#define lineRight 12
#define lineLeft 3
#define RIGHT_SENSOR 8
#define LEFT_SENSOR 9
#define FRONT_SENSOR 7

// Constants
const int ONstate = 1;
const int FRONT_BIT = 0;
const int LEFT_BIT = 1;
const int RIGHT_BIT = 2;
const int LINE_R_BIT = 3;
const int LINE_L_BIT = 4;

// Global variables
unsigned long exploreStartTime = 0;
unsigned long exploreDuration = 0;
unsigned long actionStartTime = 0;
unsigned long actionDuration = 200;
bool cond = false;
int sensorStates = 0;
int wheelSpeed = 200;

void setup() {
  Serial.begin(9600);
  pinMode(M2R, OUTPUT);
  pinMode(M2L, OUTPUT);
  pinMode(M1L, OUTPUT);
  pinMode(M1R, OUTPUT);
  pinMode(lineRight, INPUT_PULLUP);
  pinMode(lineLeft, INPUT_PULLUP);
  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(FRONT_SENSOR, INPUT);
  pinMode(JSUMO_SWITCH, INPUT);
  updateSensorStates();
}

void moveForward(int leftSpeed, int rightSpeed) {
  analogWrite(M1L, leftSpeed);
  analogWrite(M2R, rightSpeed);
  digitalWrite(M1R, LOW);
  digitalWrite(M2L, LOW);
}

void moveBackward(int leftSpeed, int rightSpeed) {
  analogWrite(M1R, leftSpeed);
  analogWrite(M2L, rightSpeed);
  digitalWrite(M1L, LOW);
  digitalWrite(M2R, LOW);
}

void turnLeft(int leftSpeed, int rightSpeed) {
  analogWrite(M1R, leftSpeed);
  analogWrite(M2R, rightSpeed);
  digitalWrite(M1L, LOW);
  digitalWrite(M2L, LOW);
}

void turnRight(int leftSpeed, int rightSpeed) {
  analogWrite(M1L, leftSpeed);
  analogWrite(M2L, rightSpeed);
  digitalWrite(M1R, LOW);
  digitalWrite(M2R, LOW);
}



void stopMovement() {
  digitalWrite(M1R, LOW);
  digitalWrite(M2L, LOW);
  digitalWrite(M1L, LOW);
  digitalWrite(M2R, LOW);
}

void snakeMovement() {
  static bool direction = true;
  static unsigned long lastChangeTime = millis();
  unsigned long currentTime = millis();

  if (currentTime - lastChangeTime >= 500) {
    direction = !direction;
    lastChangeTime = currentTime;
  }

  if (direction) {
    moveForward(255, 150);
     while (millis() - actionStartTime < 950) {
        if (!digitalRead(lineRight) || !digitalRead(lineLeft) || digitalRead(FRONT_SENSOR) || digitalRead(LEFT_SENSOR) || digitalRead( RIGHT_SENSOR))
        {break;}
     }
  } else {
    moveForward(150, 255);
     while (millis() - actionStartTime < 950) {
        if (!digitalRead(lineRight) || !digitalRead(lineLeft) || digitalRead(FRONT_SENSOR) || digitalRead(LEFT_SENSOR) || digitalRead( RIGHT_SENSOR))
        {break;}
     }
  }
}



void updateSensorStates() {
  sensorStates = (digitalRead(FRONT_SENSOR) << FRONT_BIT) |
                 (digitalRead(LEFT_SENSOR) << LEFT_BIT) |
                 (digitalRead(RIGHT_SENSOR) << RIGHT_BIT) |
                 (!digitalRead(lineRight) << LINE_R_BIT) |
                 (!digitalRead(lineLeft) << LINE_L_BIT);
}

unsigned long newc = 0;
void loop() {
  int JSUMOdata = digitalRead(JSUMO_SWITCH);
  updateSensorStates();
  newc = millis();

  switch (sensorStates) { 
    case 0:
      snakeMovement();
      break;
    case 1:  // Obstacle in front
      
        
        moveForward(255, 255);
      
      Serial.println("forward");
      break;
    case 2:  // Obstacle on the left

        turnLeft(255,255);
        while (millis() - actionStartTime < 1050) {
          if (digitalRead(lineLeft) || digitalRead(lineRight)|| digitalRead(FRONT_SENSOR)) {
            break;
          }
        
      }
      break;
    case 3:  // Obstacle detected by front and left sensors
    
        moveForward(155, 255);
       
      
      break;
    case 4:  // Obstacle on the right
        turnRight(255,255);
        while (millis() - actionStartTime < 1050) {
          if (digitalRead(lineLeft) || digitalRead(lineRight) || digitalRead(FRONT_SENSOR)) {
            break;
          }
        }
      
      break;
    case 5:  // Obstacle detected by front and right sensors
     moveForward(255,155);
        
      
      break;
    case 24:  // Both line sensors
      moveBackward(255, 255);
      actionStartTime = millis();
      while (millis() - actionStartTime < 500) {
        if (digitalRead(FRONT_SENSOR) || digitalRead(LEFT_SENSOR) || digitalRead( RIGHT_SENSOR)) {
          break;
        }}
        // Wait for the duration
      
      turnRight(255, 255);
      actionStartTime = millis();
      while (millis() - actionStartTime < 390) {
        if (digitalRead(FRONT_SENSOR) || digitalRead(LEFT_SENSOR) || digitalRead( RIGHT_SENSOR)) {
          break;
        }
        // Wait for the duration
      }
      break;
    case 8:  // Right line sensor
      moveBackward(255, 255);
      actionStartTime = millis();
      while (millis() - actionStartTime < 500) {
        if (digitalRead(FRONT_SENSOR) || digitalRead(LEFT_SENSOR) || digitalRead( RIGHT_SENSOR)) {
          break;
        }
        // Wait for the duration
      }
      turnRight(255, 255);
      actionStartTime = millis();
      while (millis() - actionStartTime < 390) {
        if (digitalRead(FRONT_SENSOR) || digitalRead(LEFT_SENSOR) || digitalRead( RIGHT_SENSOR)) {
          break;
        }
        // Wait for the duration
      }
      break;
    case 16:  // Left line sensor
      moveBackward(255, 255);
      actionStartTime = millis();
      while (millis() - actionStartTime < 500) {
        if (digitalRead(FRONT_SENSOR) || digitalRead(LEFT_SENSOR) || digitalRead( RIGHT_SENSOR)) {
          break;
        }
        // Wait for the duration
      }
      turnLeft(255, 255);
      actionStartTime = millis();
      while (millis() - actionStartTime < 390) {
        if (digitalRead(FRONT_SENSOR) || digitalRead(LEFT_SENSOR) || digitalRead( RIGHT_SENSOR)) {
          break;
        }
      }
      break;
    case 25:
    case 17:
    case 9: 
    case 26: 
    case 27: 
    case 28: 
    case 29: 
      stopMovement();
      break;
  }
}
