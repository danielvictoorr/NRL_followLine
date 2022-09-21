#include <MotorEncoderNRL.h>
#include <SeguidorNRL.h>
#include "robotDefines.h"

MotorEncoderNRL MotorA(PIN1_MOTOR_A, PIN2_MOTOR_A, PIN_PWM_MOTOR_A, 
                       PIN1_ENCODER_MOTOR_A, PIN2_ENCODER_MOTOR_A, 
                       DIAMETRO_RODA, PPV);
MotorEncoderNRL MotorB(PIN1_MOTOR_B, PIN2_MOTOR_B, PIN_PWM_MOTOR_B, 
                       PIN1_ENCODER_MOTOR_B, PIN2_ENCODER_MOTOR_B, 
                       DIAMETRO_RODA, PPV);

SeguidorNRL Seguidor (pinSensors, MotorA, MotorB, 1); // 0 para linha branca e 1 para linha preta

void auxInterruptA(){
  MotorA.pulsoCont();
}

void auxInterruptB(){
  MotorB.pulsoCont();
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(PIN1_ENCODER_MOTOR_A), auxInterruptA, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN1_ENCODER_MOTOR_B), auxInterruptB, RISING);

  MotorA.setP(1.0);
  MotorB.setP(1.0);

  Seguidor.setPID(250.0,0,0);
  Seguidor.setLimiarSensor(600);
  Seguidor.setVelNominal(1000);

  Serial.begin(115200);
}

void loop() {
  Seguidor.checkSensors(0); // 0 para digital e 1 para analógico
  Seguidor.printError();
  Seguidor.motorControl();
}