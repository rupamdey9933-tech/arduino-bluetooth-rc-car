#include <SoftwareSerial.h>

SoftwareSerial BT(2, 4); 

// Motor pins
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;
const int ENA = 5;
const int ENB = 6;

char command;
int motorSpeed = 200; // Default speed (0-255)

void setup() {
  BT.begin(9600);
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  stopMotors(); // Start in stopped state
}

// -------- MOVEMENT FUNCTIONS --------
void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  applySpeed();
}

void backward() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  applySpeed();
}

void left() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); 
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  
  applySpeed();
}

void right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); 
  applySpeed();
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// -------- SPEED CONTROL --------
void applySpeed() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void loop() {
  if (BT.available() > 0) {
    command = BT.read();
    
    // Check if the command is a number for speed (0 to 9)
    // 'q' is often used by apps for 100% speed
    if (command >= '0' && command <= '9') {
      motorSpeed = map(command - '0', 0, 9, 0, 255);
      applySpeed();
      Serial.print("Speed set to: ");
      Serial.println(motorSpeed);
    } 
    else if (command == 'q') {
      motorSpeed = 255;
      applySpeed();
    }
    
    // Movement commands
    switch (command) {
      case 'F': forward();    break;
      case 'B': backward();   break;
      case 'L': right();       break;
      case 'R': left();      break;
      case 'S': stopMotors(); break;
    }
  }
}
