#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// =====================================
// L298N Motor Driver Pins
// =====================================

// Left Motor
#define ENA 25
#define IN1 26
#define IN2 27

// Right Motor
#define ENB 14
#define IN3 32
#define IN4 33

// Relay for Vacuum Motor
#define RELAY_PIN 23


// =====================================
// PWM Settings
// =====================================

#define PWM_FREQ 1000
#define PWM_RESOLUTION 8

int motorSpeed = 200;


// =====================================
// Setup
// =====================================

void setup() {

  Serial.begin(115200);

  // Bluetooth Device Name
  SerialBT.begin("ESP32_Vacuum_Car");

  // L298N Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Relay Pin
  pinMode(RELAY_PIN, OUTPUT);

  // =====================================
  // PWM Setup for L298N
  // =====================================

  ledcAttach(ENA, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(ENB, PWM_FREQ, PWM_RESOLUTION);

  // Initial State
  stopCar();
  vacuumOff();

  Serial.println("================================");
  Serial.println("ESP32 Vacuum Cleaner Car Ready");
  Serial.println("Bluetooth Name: ESP32_Vacuum_Car");
  Serial.println("================================");
}


// =====================================
// Main Loop
// =====================================

void loop() {

  if (SerialBT.available()) {

    char command = SerialBT.read();

    Serial.print("Received: ");
    Serial.println(command);

    switch (command) {

      // Car Control
      case 'F':
        forward();
        break;

      case 'B':
        backward();
        break;

      case 'L':
        left();
        break;

      case 'R':
        right();
        break;

      case 'S':
        stopCar();
        break;

      // Vacuum Control
      case 'V':
        vacuumOn();
        break;

      case 'v':
        vacuumOff();
        break;

      // Speed Control
      case '1':
        motorSpeed = 100;
        SerialBT.println("Speed: LOW");
        break;

      case '2':
        motorSpeed = 180;
        SerialBT.println("Speed: MEDIUM");
        break;

      case '3':
        motorSpeed = 255;
        SerialBT.println("Speed: HIGH");
        break;
    }
  }
}


// =====================================
// Forward
// =====================================

void forward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(ENA, motorSpeed);
  ledcWrite(ENB, motorSpeed);

  SerialBT.println("CAR: FORWARD");
}


// =====================================
// Backward
// =====================================

void backward() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(ENA, motorSpeed);
  ledcWrite(ENB, motorSpeed);

  SerialBT.println("CAR: BACKWARD");
}


// =====================================
// Left
// =====================================

void left() {

  // Left Motor Backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Right Motor Forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(ENA, motorSpeed);
  ledcWrite(ENB, motorSpeed);

  SerialBT.println("CAR: LEFT");
}


// =====================================
// Right
// =====================================

void right() {

  // Left Motor Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Right Motor Backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(ENA, motorSpeed);
  ledcWrite(ENB, motorSpeed);

  SerialBT.println("CAR: RIGHT");
}


// =====================================
// Stop
// =====================================

void stopCar() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);

  SerialBT.println("CAR: STOP");
}


// =====================================
// Vacuum ON
// =====================================

void vacuumOn() {

  // সাধারণত বেশিরভাগ relay module
  // Active LOW হয়
  digitalWrite(RELAY_PIN, LOW);

  SerialBT.println("VACUUM: ON");
}


// =====================================
// Vacuum OFF
// =====================================

void vacuumOff() {

  digitalWrite(RELAY_PIN, HIGH);

  SerialBT.println("VACUUM: OFF");
}