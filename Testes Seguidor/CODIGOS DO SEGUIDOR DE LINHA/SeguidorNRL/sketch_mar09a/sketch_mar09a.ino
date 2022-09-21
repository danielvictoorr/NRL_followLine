#include "defines.h"

void setup() {
  pinMode(PIN1_MOTOR_A,OUTPUT);
  pinMode(PIN2_MOTOR_A,OUTPUT);
  pinMode(PIN1_MOTOR_B,OUTPUT);
  pinMode(PIN2_MOTOR_B,OUTPUT);

  pinMode(PINPWM_MOTOR_A,OUTPUT);
  pinMode(PINPWM_MOTOR_B,OUTPUT);
  analogWrite(PINPWM_MOTOR_A, 150);
  analogWrite(PINPWM_MOTOR_B, 150);
  Serial.begin(115200);

}

void loop() {
  digitalWrite(PIN2_MOTOR_A, HIGH);
  digitalWrite(PIN1_MOTOR_A, LOW);
  Serial.println("Frente");
  delay(2000);
  digitalWrite(PIN1_MOTOR_B, LOW);
  digitalWrite(PIN2_MOTOR_B, HIGH);
  Serial.println("Tras");
  delay(2000);
  
}
