#define Kp_Motor 4
#define Kd_Motor 0.3
#define Ki_Motor 0.0

float erroAntA = 0, erroAntB = 0, velAtualA = 0, velAtualB = 0, pwmVelA = 0, pwmVelB = 0;
float P_MotorA = 0, I_MotorA = 0, D_MotorA = 0;
float P_MotorB = 0, I_MotorB = 0, D_MotorB = 0;
long tAnteriorA = 0, tAnteriorB = 0;
int PWM_MotorA = 0, PWM_MotorB = 0, pulseCountA = 0, pulseCountB = 0;

int countEeprom = 0; // <--- MERDA

float PID_MotorA(float velSetPointA) {
  float erroVelA = velSetPointA - velAtualA;
  P_MotorA = erroVelA;
  if (!(PWM_MotorA >= 255)) /* --- Anti windup --- */
    I_MotorA = I_MotorA + erroVelA;
  D_MotorA = erroVelA - erroAntA;
  erroAntA = erroVelA;
  return (Kp_Motor * P_MotorA) + (Ki_Motor * I_MotorA) + (Kd_Motor * D_MotorA);
}

float PID_MotorB(float velSetPointB) {
  float erroVelB = velSetPointB - velAtualB;
  P_MotorB = erroVelB;
  if (!(PWM_MotorB >= 255)) /* --- Anti windup --- */
    I_MotorB = I_MotorB + erroVelB;
  D_MotorB = erroVelB - erroAntB;
  erroAntB = erroVelB;
  return (Kp_Motor * P_MotorB) + (Ki_Motor * I_MotorB) + (Kd_Motor * D_MotorB);
}

int updateVel(int velA, int velB) {
  PWM_MotorA = PID_MotorA(velA);
  if (PWM_MotorA >= 255) PWM_MotorA = 255;
  if (PWM_MotorA <= -255) PWM_MotorA = -255;
  if (PWM_MotorA <= 0) {
    digitalWrite(PIN1_MOTOR_A, HIGH);  //Direção motor A
    digitalWrite(PIN2_MOTOR_A, LOW);
  }
  else {
    digitalWrite(PIN1_MOTOR_A, LOW);  //Direção motor A
    digitalWrite(PIN2_MOTOR_A, HIGH);
  }
  analogWrite(PINPWM_MOTOR_A, abs(PWM_MotorA));
  //analogWrite(PINPWM_MOTOR_A, 255);

  PWM_MotorB = PID_MotorB(velB);
  if (PWM_MotorB >= 255) PWM_MotorB = 255;
  if (PWM_MotorB <= -255) PWM_MotorB = -255;
  if (PWM_MotorB <= 0) {
    digitalWrite(PIN1_MOTOR_B, LOW);  //Direção motor B
    digitalWrite(PIN2_MOTOR_B, HIGH);
  }
  else {
    digitalWrite(PIN1_MOTOR_B, HIGH);  //Direção motor B
    digitalWrite(PIN2_MOTOR_B, LOW);
  }
  analogWrite(PINPWM_MOTOR_B, abs(PWM_MotorB));
  //analogWrite(PINPWM_MOTOR_B, 255);

  //Serial.println(String(abs(velAtualA)) + "," + String(abs(velAtualB)));
}

void interruptEncoderA() {
  /* --- Calcula a diferença de tempo entre pulsos em segundos --- */
  float DTa = (micros() - tAnteriorA) / 1000000.0; 
  tAnteriorA = micros();
  
  /* --- Calcula a velocidade instantânea dx/dt --- */
  velAtualA = ((double)DX / (double)DTa);

  /* --- Realiza a leitura do segundo sensor do encoder para determinar a direção de rotação da roda --- */
  if(digitalRead(PIN2_ENCODER_A))
    velAtualA *= -1;

  pulseCountA++;
}

void interruptEncoderB() {
  /* --- Calcula a diferença de tempo entre pulsos em segundos --- */
  float DTb = (micros() - tAnteriorB) / 1000000.0;
  tAnteriorB = micros();

  /* --- Calcula a velocidade instantânea dx/dt --- */
  velAtualB = ((double)DX / (double)DTb);

  /* --- Realiza a leitura do segundo sensor do encoder para determinar a direção de rotação da roda --- */
  if(digitalRead(PIN2_ENCODER_B))
    velAtualB *= -1;

  pulseCountB++;
}

void stopRobot() { /* --- Desliga os dois motores --- */
  STATUS = PARADO;
//  digitalWrite(PIN1_MOTOR_A, LOW);
//  digitalWrite(PIN2_MOTOR_A, LOW);
//  digitalWrite(PIN1_MOTOR_B, LOW);
//  digitalWrite(PIN2_MOTOR_B, LOW);

  analogWrite(PINPWM_MOTOR_A, 0);
  analogWrite(PINPWM_MOTOR_B, 0);
}
