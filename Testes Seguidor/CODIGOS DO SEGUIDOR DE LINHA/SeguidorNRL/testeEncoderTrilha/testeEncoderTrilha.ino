#include "MotorCtrl.h"

void interup1();
void interup2();

MotorCtrl motor1(9, 8, 10, 3, -1, ((37 * PI) / 15), interup1);
MotorCtrl motor2(6, 7, 5, 2, -1, ((37 * PI) / 15), interup2);
int vel;
int i = 0;
float soma;

int pin1=2, pin2=3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motor1.setKP(1);
  motor2.setKP(1);
  //pinMode(pin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin1),interup1,FALLING);

  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
}


void loop() {
/*
  Serial.print(digitalRead(pin1)+2);
  Serial.print(",");
  Serial.println(digitalRead(pin2));
  /*
  motor1.updateCtrl();
  motor2.updateCtrl();

  if (millis() > 3000) {
    motor1.setSpeed(1000);
    motor2.setSpeed(1000);
  }
  */
  /*
  // put your main code here, to run repeatedly:
  soma += (float)motor1.getVel() / (0.00001+(float)motor2.getVel());
  i++;
  if (i >= 9) {
    //Serial.println(soma / 10);
    i = 0;
    soma=0;
  }
  Serial.print(motor1.getVel());
    Serial.print(",");
    Serial.println(motor2.getVel());
  //  Serial.print(",");
  //  Serial.println(digitalRead(pin2)+2);
  */
}

void interup1() {
  motor1.encoderPulse();
  //  if (digitalRead(pin2))
  //    Serial.println(digitalRead(pin2));
  //  else
  //    Serial.println(digitalRead(pin2));
  Serial.println(digitalRead(pin2));
}
void interup2() {
  motor2.encoderPulse();
  //  if (digitalRead(pin2))
  //    Serial.println(digitalRead(pin2));
  //  else
  //    Serial.println(digitalRead(pin2));
}

