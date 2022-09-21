#include "define.h"

float P = 0, I = 0, D = 0, erroAnterior = 0; // // Aqui fica as variaveis que armazenaram os erros para o PID 
int auxInicioFim = 0, indexInicioFim = 0,auxCurva = 0, indexCurva = 0, auxBotao1 = 0, auxBotao2 = 0, valueSensor = 0;
int pwm_A = 0, pwm_B = 0;
int timerBuzz = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");
  setupPins();
}

void loop() {
  
  
  
  statusUpdate();
  Inicio_Fim();
  if(STATUS == SEGUINDO){
    digitalWrite(PIN1_MOTOR_A, HIGH);
      digitalWrite(PIN2_MOTOR_A, LOW);
      digitalWrite(PIN1_MOTOR_B, HIGH);
      digitalWrite(PIN2_MOTOR_B, LOW);

      //
      identificaCurva();
      if(indexCurva > NUM_SEGMENTOS-1){
        indexCurva = NUM_SEGMENTOS-1;
      }
      int maxPWM = VEL_CURVA[indexCurva];
      Serial.println(maxPWM);
      int PID = calculatePID();
      pwm_A = maxPWM  + PID;
      pwm_B = maxPWM  - PID;
      //Serial.print(pwm_A);Serial.print(" "); Serial.println(pwm_B);
      analogWrite(PINPWM_MOTOR_A, pwm_A/1.5);
      analogWrite(PINPWM_MOTOR_B, pwm_B/1.5);
  }
}

void statusUpdate() {
  int b1 = digitalRead(BOTAO1);
  int b2 = digitalRead(BOTAO2);

  // BOTÃO 1 #########################
  if (b1 == 0 && auxBotao1 == 0) {
    STATUS = SEGUINDO;
    //if (buzzerQtd == 0) buzzerQtd = 2 * 2;

    auxBotao1 = 1;
    delay(100);
  }
  else if (b1 == 1)
    auxBotao1 = 0;

  // BOTÃO 2 #########################
  if (b2 == 0 && auxBotao2 == 0) {
    /*if (buzzerQtd == 0) buzzerQtd = 2 * (presetVel + 1);
    presetVel++;
    if (presetVel == 3)
      presetVel = 0;*/

    auxBotao2 = 1;
    delay(100);
  }
  else if (b2 == 1)
    auxBotao2 = 0;
}



int calculatePID() {
  int acao, erro = getError();
  P = erro;
  I = I + erro;
  D = erro - erroAnterior;
  acao = (KP * P) + (KI * I) + (KD * D);
  erroAnterior = erro;
  return acao;
}

int getError() {
  /*
     1 0 0 0 0
     1 1 0 0 0
     0 1 0 0 0
     0 1 1 0 0
     0 0 1 0 0
     0 0 1 1 0
     0 0 0 1 0
     0 0 0 1 1
     0 0 0 0 1
  */

  int valueSensores[7], erro;

  for (int i = 0; i < 7; i++) {
    if (analogRead(PIN_SENSORS[i]) > LIMIAR_SENSOR)
      valueSensores[i] = (COR_LINHA == 0) ? 1 : 0;
    else
      valueSensores[i] = (COR_LINHA == 0) ? 0 : 1;

    Serial.print(analogRead(PIN_SENSORS[i]));
    Serial.print(" , ");
  }
  Serial.println();

  if (valueSensores[1] == 1 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -4;
  else if (valueSensores[1] == 1 && valueSensores[2] == 1 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -3;
  else if (valueSensores[1] == 0 && valueSensores[2] == 1 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -2;
  else if (valueSensores[1] == 0 && valueSensores[2] == 1 && valueSensores[3] == 1 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -1;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 1 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = 0;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 1 && valueSensores[4] == 1 && valueSensores[5] == 0) erro = 1;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 1 && valueSensores[5] == 0) erro = 2;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 1 && valueSensores[5] == 1) erro = 3;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 1) erro = 4;

  return erro;
}

void setupPins() {
  pinMode(BOTAO1, INPUT_PULLUP);
  pinMode(BOTAO2, INPUT_PULLUP);

  pinMode(PIN1_MOTOR_A, OUTPUT);
  pinMode(PIN2_MOTOR_A, OUTPUT);
  pinMode(PINPWM_MOTOR_A, OUTPUT);

  pinMode(PIN1_MOTOR_B, OUTPUT);
  pinMode(PIN2_MOTOR_B, OUTPUT);
  pinMode(PINPWM_MOTOR_B, OUTPUT);

  for (int i = 0; i < 7; i++)
      pinMode(PIN_SENSORS[i], INPUT_PULLUP);
}

//void identificaCurva(){
//    int valueSensor = 0;
//    if (analogRead(PIN_SENSORS[0])>LIMIAR_SENSOR) valueSensor = ((COR_LINHA == 0)?1:0);
//    else  valueSensor = ((COR_LINHA == 0)?0:1);
//    
//    if(valueSensor == 1 && auxCurva == 0){
//     indexCurva++;
//     Serial.println(indexCurva);
//     auxCurva = 1;
//    }
//    else if (valueSensor == 0)
//      auxCurva = 0;  
//  }

  void identificaCurva(){
      if (analogRead(PIN_SENSORS[0])>LIMIAR_SENSOR){
            valueSensor = 0;
            auxCurva = 0; 
      }
      else{  // if(analogRead(PIN_SENSORS[0]<LIMIAR_SENSOR))
        valueSensor = 1;
         //indexCurva++;
         //auxCurva = 1;       
      }
      if(valueSensor == 1 && auxCurva == 0){
        indexCurva++;
        auxCurva = 1;
      }
      Serial.println(indexCurva);
  }

  void Inicio_Fim(){
        int valueSensor = 0;
        if(analogRead(PIN_SENSORS[0]>LIMIAR_SENSOR)) valueSensor = (COR_LINHA == 0)? 1:0;
        else valueSensor = (COR_LINHA == 0) ? 0:1;

        if(valueSensor == 1 && auxInicioFim == 0){
          indexInicioFim = indexInicioFim + 1;
        }

        if(indexInicioFim == (NUM_CRUZAMENTO + 2)){
          STATUS = PARADO;
          digitalWrite(PIN1_MOTOR_A, LOW);
          digitalWrite(PIN2_MOTOR_A, LOW);
          digitalWrite(PIN1_MOTOR_B, LOW);
          digitalWrite(PIN2_MOTOR_B, LOW);
        }
    }

//void Bipe() {
  //timerBuzz = millis();
//}
