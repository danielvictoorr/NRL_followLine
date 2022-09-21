#include <MotorEncoderNRL.h>

#define PIN1_MOTOR_A 6
#define PIN2_MOTOR_A 7
#define PIN_PWM_MOTOR_A 5
#define PIN1_ENCODER_MOTOR_A 5
#define PIN2_ENCODER_MOTOR_A 5
#define PIN1_MOTOR_B 8
#define PIN2_MOTOR_B 9
#define PIN_PWM_MOTOR_B 5
#define PIN1_ENCODER_MOTOR_B 5
#define PIN2_ENCODER_MOTOR_B 5

#define DIAMETRO_RODA 40
#define PPV 15


MotorEncoderNRL MotorA(PIN1_MOTOR_A, PIN2_MOTOR_A, PIN_PWM_MOTOR_A, 
					   PIN1_ENCODER_MOTOR_A, PIN2_ENCODER_MOTOR_A, 
					   DIAMETRO_RODA, PPV);
MotorEncoderNRL MotorB(PIN1_MOTOR_B, PIN2_MOTOR_B, PIN_PWM_MOTOR_B, 
					   PIN1_ENCODER_MOTOR_B, PIN2_ENCODER_MOTOR_B, 
					   DIAMETRO_RODA, PPV);

void auxInterruptA(){
  MotorA.pulsoCont();
}

void auxInterruptB(){
  MotorB.pulsoCont();
}

void setup() {
  attachInterrupt(0, auxInterruptA, RISING);
  attachInterrupt(1, auxInterruptB, RISING);
  MotorA.setP(1);
  MotorB.setP(1);
}

void loop() {
  MotorA.updateVel();
  MotorB.updateVel();
}
