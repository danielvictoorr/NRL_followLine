#include <EEPROM.h  >
#include "defines3.h"
#include "pinos3.h"
#include "MotorCtrl.h"

float P = 0, I = 0, D = 0, erroAnterior = 0; /* --- Variáveis relacionadas ao PID geral do robô --- */
int indexInicioFim = 0, indexCurva = 0, indexVel = 0, /* --- Variáveis contadoras de controle --- */
    auxBotao1 = 0, auxBotao2 = 0, auxInicioFim = 0, auxVel = 0, auxCurva = 0, /* --- Variáveis auxiliares --- */
    timeBuzz; /* --- Variável de controle do tempo do buzzer --- */
float distancia[NUM_SEGMENTOS], distanPer = 0;
int verificaReta = 0;
long erro = 0;

void setup() {
  Serial.begin(115200); /* --- Iniciando a serial --- */
  Serial.println("Setup");

  setupPins(); /* --- Chamada de função para configuração dos pinos --- */

  /* --- Lendo dados da EEPROM relacionados à distância dos trechos --- */
  int addr = 0;
  for (int i = 0; i < NUM_SEGMENTOS; i++) {
    EEPROM.get(addr, distancia[i]);
    Serial.println("Trecho " + String(i) + " addr " + String(addr) + " : " + String(distancia[i]) + " cm");
    addr = addr + sizeof(distancia[i]);
  }
  addr = addr - sizeof(distancia[0]);
  Serial.println();
  for (int i = 0; i < 5; i++) {
    EEPROM.get(addr, MIN_SENSOR[i]);
    Serial.println("Mínimo sensor " + String(i) + ": " + String(MIN_SENSOR[i]));
    addr = addr + sizeof(MIN_SENSOR[i]);
  }
  Serial.println();
  for (int i = 0; i < 5; i++) {
    EEPROM.get(addr, MAX_SENSOR[i]);
    Serial.println("Máximo sensor " + String(i) + ": " + String(MAX_SENSOR[i]));
    addr = addr + sizeof(MAX_SENSOR[i]);
  }
  Serial.println();
  
  /* --- Estrutura para aplicar os multiplicadores de velocidade nas retas e curvas --- */
  int j=0;
  for (int i = 0; i < NUM_SEGMENTOS; i++) {
    if(SEG_RETA[j] != i ){
      VEL_CURVA[i] *=  MultiplicadorCurva;
      Serial.println(String(i) + " é curva");
    }
    else{
      VEL_CURVA[i] *=  MultiplicadorReta;
      j++;
      Serial.println(String(i) + " é reta");
    }
  }
  delay(100);
  bipeBuzzer();
  indexCurva = 0;
}

void loop() {
  /*int H[5], soma = 0, media = 0;
  for (int i = 0; i < 5; i++) {
    H[i] = map(analogRead(PIN_SENSORS[i+1]), MIN_SENSOR[i], MAX_SENSOR[i], 0, 100);
    //media += (H[i] * i * 100);
    soma += H[i];

    Serial.print(String(H[i]) + ",");
  }

  media = -24*(H[0]-H[4]) -12*(H[1]-H[3]) -0*H[2];

  Serial.println("      " + String(media/soma));*/
//  Serial.begin(115200); /* --- Iniciando a serial --- */
//  verificaReta = 1;
//  while(1){
//    getError();
//    delay(1);
//  }
  
  updateBuzzer(); /* --- Chamada de função de update do tempo de acionamento do buzzer --- */
  statusUpdate(); /* --- Chamada de função para ler os botões e atualizar o status ro robô --- */

  if (STATUS == SEGUINDO) { /* --- Condição para status SEGUINDO do robô --- */
    odometroUpdate(); /* --- Chamada de função para realização da odometria --- */
    identificaCurva(); /* --- Chamada de função para identificação da flag de curva --- */
    identificaInicioFim(); /* --- Chamada de função para identificação da flag de inicio e fim de pista --- */
    changeVel(); /* --- Chamada de função para atualização da velocidade base do robô --- */

    /* --- Tratamento para a variável indexVel não ultrapassar o numero de 
           seguimentos e aplicar um lixo de memória como velocidade do robô --- */
    if (indexVel > NUM_SEGMENTOS - 1)
      indexVel = NUM_SEGMENTOS - 1;

    /* --- Chamada de função para calcular a ação do PID sobre a velocidade das rodas do robô --- */
    int PID = calculatePID(); 

    /* --- Aplicando o diferencial de velocidades nas rodas de acordo com a ação do PID --- */
    float velMotorA = VEL_CURVA[indexVel] + PID; 
    float velMotorB = VEL_CURVA[indexVel] - PID; 

//    float vel = VEL_CURVA[indexVel]/(abs(erro/250)+1);
//
//    if(vel < PID) vel = PID;
//    
//    float velMotorA = vel + PID; 
//    float velMotorB = vel - PID; 
    
    if (velMotorA < 0) velMotorA = 0;
    if (velMotorB < 0) velMotorB = 0;

    /* --- Chamada de função para atualizar o controle de velocidade das rodas --- */
    updateVel(velMotorA, velMotorB);
    //updateVel(0, 0); /* --- Teste de velocidade = 0 --- */
  }
  else if (STATUS == CALIBRANDO) {

    //int valueSensores[5];
    long soma = 0;
    int nAmostras = 20;

    Serial.println("Mínimos:");
    for (int v = 0; v < 5; v++) {
      soma = 0;
      for (int i = 0; i < nAmostras; i++) {
        //valueSensores[v] = analogRead(PIN_SENSORS[v+1]);
        //if(valueSensores[i] < MIN_SENSOR[i]) MIN_SENSOR[i] = valueSensores[i];
        soma += analogRead(PIN_SENSORS[v+1]);
      }
      soma /= nAmostras;
      MIN_SENSOR[v] = soma;
      Serial.println(MIN_SENSOR[v]);
    }

    for(int v=0; v<2; v++){
      digitalWrite(BUZZ, HIGH);
      delay(100);
      digitalWrite(BUZZ, LOW);
      delay(100);
    }
    delay(2000);

    Serial.println("\nMáximos:");
    for (int v = 0; v < 5; v++) {
      soma = 0;
      for (int i = 0; i < nAmostras; i++) {
        //valueSensores[v] = analogRead(PIN_SENSORS[v+1]);
        //if(valueSensores[v] > MAX_SENSOR[v]) MAX_SENSOR[v] = valueSensores[i];
        soma += analogRead(PIN_SENSORS[v+1]);

        delay(10);
      }
      soma /= nAmostras;
      MAX_SENSOR[v] = soma;
      Serial.println(MAX_SENSOR[v]);
    }

    //LIMIAR_SENSOR = (MAX_SENSOR[0] + MIN_SENSOR[0])/2;

    for(int v=0; v<2; v++){
      digitalWrite(BUZZ, HIGH);
      delay(100);
      digitalWrite(BUZZ, LOW);
      delay(100);
    }
    
    int addr = (NUM_SEGMENTOS-1)*4;
    for (int i = 0; i < 5; i++) {
      EEPROM.put(addr, MIN_SENSOR[i]);
      addr += sizeof(MIN_SENSOR[i]);
      //Serial.println(String(MIN_SENSOR[i]));
      //Serial.println(" - " + String(addr));
    }
    Serial.println();
    for (int i = 0; i < 5; i++) {
      EEPROM.put(addr, MAX_SENSOR[i]);
      addr += sizeof(MIN_SENSOR[i]);
//      Serial.println(String(MAX_SENSOR[i]));
//      Serial.println(" - " + String(addr));
    }
    //Serial.println();

    STATUS = PARADO;
  }
//  else if (STATUS == PARADO){
//    //updateVel(0, 0);
//  }
}

void statusUpdate() {
  //Serial.println("statusUpdate   " + String(indexCurva) + " " + String(STATUS));
  if(indexCurva == 0 && STATUS == PARADO){
    
    /* --- Converte a informação analogica do sensor em digital 
          de acordo com o limiar definido e da cor da linha --- */
    int valueSensores[7];
    for (int i = 0; i < 7; i++) {
      if (analogRead(PIN_SENSORS[i]) > LIMIAR_SENSOR) valueSensores[i] = (COR_LINHA == 0) ? 1 : 0;
      else valueSensores[i] = (COR_LINHA == 0) ? 0 : 1;
    }

    /* --- Caso tenha passado pela flag de inicio e fim de pista ou cruzamento --- */
    if (valueSensores[6] == 1 && auxBotao1 == 0) {
      for(int v=0; v<3; v++){
        digitalWrite(BUZZ, HIGH);
        delay(100);
        digitalWrite(BUZZ, LOW);
        delay(100);
      }
      delay(3000);
      STATUS = SEGUINDO;
      auxBotao1 = 1;
    }
    else if (valueSensores[6] == 0) {
      auxBotao1 = 0;
    }

    if (valueSensores[0] == 1 && auxBotao2 == 0) {
      for(int v=0; v<5; v++){
        digitalWrite(BUZZ, HIGH);
        delay(100);
        digitalWrite(BUZZ, LOW);
        delay(100);
      }
      delay(3000);
      STATUS = CALIBRANDO;
      auxBotao2 = 1;
    }
    else if (valueSensores[0] == 0) {
      auxBotao2 = 0;
    }
  }
}

int calculatePID() {
  int acao, erro = getError(); /* --- Chamada de função para retorno do erro dos sensores --- */
  P = erro;                /* --- Termo proporcional --- */
  I = I + erro;            /* --- Termo integrativo --- */
  D = erro - erroAnterior; /* --- Termo derivativo --- */
  acao = (KP * P) + (KI * I) + (KD * D); /* --- Cálculo da ação do PID --- */
  erroAnterior = erro; /* --- Salva o erro para o próximo cálculo do derivativo --- */
  return acao;
}

void identificaCurva() {
  /* --- Converte a informação analogica do sensor em digital 
         de acordo com o limiar definido e da cor da linha --- */
  int valueSensor = 0;
  if (analogRead(PIN_SENSORS[0]) > LIMIAR_SENSOR) valueSensor = ((COR_LINHA == 0) ? 1 : 0);
  else valueSensor = ((COR_LINHA == 0) ? 0 : 1);

  /* --- Caso tenha passado pela flag de curva --- */
  if (valueSensor == 1 && auxCurva == 0) {
    indexCurva++; /* --- Adiciona uma unidade à contagem de curvas --- */
    //bipeBuzzer(); /* --- Bipa o buzzer para identificação auditiva --- */
    pulseCountA = 0; /* --- Reseta a variável contadora de pulsos do motor A --- */
    pulseCountB = 0; /* --- Reseta a variável contadora de pulsos do motor B --- */
    distanPer = 0; /* --- Reseta a variável contadora de distância percorrida pelo robô --- */
    //Serial.println(indexCurva);

    /* --- Caso o robô tenha passado pela flag mas ainda não mudou de velocidade, 
           adiciona uma unidade na contagem no índice de velocidade.
           Usado para corrigir o dilema onde o trecho é menor que o gap de velocidade. --- */
    if (auxVel == 0) 
      indexVel++;

    auxVel = 0;
    auxCurva = 1;
  }
  else if (valueSensor == 0)
    auxCurva = 0;
}

void identificaInicioFim() {
  /* --- Converte a informação analogica do sensor em digital 
         de acordo com o limiar definido e da cor da linha --- */
  int valueSensor = 0;
  if (analogRead(PIN_SENSORS[6]) > LIMIAR_SENSOR) valueSensor = (COR_LINHA == 0) ? 1 : 0;
  else valueSensor = (COR_LINHA == 0) ? 0 : 1;

  /* --- Caso tenha passado pela flag de inicio e fim de pista ou cruzamento --- */
  if (valueSensor == 1 && auxInicioFim == 0) {
    indexInicioFim++; /* --- Adiciona uma unidade à contagem de inicio e fim ou cruzamentos --- */
    bipeBuzzer(); /* --- Bipa o buzzer para identificação auditiva --- */
    auxInicioFim = 1;
  }
  else if (valueSensor == 0) {
    auxInicioFim = 0;
  }

  if(indexInicioFim == 1 && indexCurva == 0){
    indexVel++;
    indexCurva++;
  } 

  if (indexInicioFim == (NUM_CRUZAMENTO + 2)) { /* --- Caso o robô tenha completado a pista --- */
    stopRobot(); /* --- Chamada de função para parar o robô --- */
    
    Serial.println("Parou! " + String(indexInicioFim));
    while (1) { /* --- Trava o código para não atualizar as velocidade dos motores --- */
      updateBuzzer();
      statusUpdate();
    }
  }
}

void changeVel() {
  /* --- Variável que indica quantos centímetros antes do trecho o robô deve alterar a velocidade base.
         Chamada de gap de velocidade. --- */
  float gap = 20.0;
  /* --- Identifica quando a velocidade do próximo trecho é menor que a atual --- */
  if (VEL_CURVA[indexVel + 1] < VEL_CURVA[indexVel]) { // Desaceleração
    /*--- A medidade do gap na desaceleração é propocional à diferença entre as velocidades --- */
    gap = 5;//(VEL_CURVA[indexVel] - VEL_CURVA[indexVel+1])/10; 
  }
  else { // Aceleração
    gap = 0; /* --- Valor do gap em condição de aceleração --- */
  }

  /* --- Condição que controla a passagem de velocidade de acordo com a distância percorrida pelo robô --- */
  if ((distancia[indexCurva] - distanPer <= gap) && (auxVel == 0) && (indexVel < NUM_SEGMENTOS - 1)) {
    indexVel++;
    auxVel = 1;
  }

  /* --- Estrutura para alternar entre os valor de KP, KI e KD para retas e curvas --- */ 
  verificaReta = 0;
  for (int i = 0; i < sizeof(SEG_RETA)/4; i++) {
    if (indexVel == SEG_RETA[i])
      verificaReta = 1;
  }

  if (verificaReta == 1) {
    KP = KP_RETA;
    KI = KI_RETA;
    KD = KD_RETA;
    //float velMedia = (velAtualA + velAtualB)/2;

//    KP = VEL_CURVA[indexVel] / 100;
//    KI = 0;
//    KD = VEL_CURVA[indexVel] / 160;
  }
  else {
    KP = KP_CURVA;
    KI = KI_CURVA;
    KD = KD_CURVA;
  }

  //Serial.println("Kp: " + String(KP) + "   Ki: " + String(KI) + "   Kd: " + String(KD));
}

void bipeBuzzer() {
  timeBuzz = millis(); /* --- Reset do tempo do buzzer --- */
}

void odometroUpdate() {
  /* --- Cálculo da distância percorrida pelo robô dada pela média 
         da contagem de pulsos dos dois encoders multiplicada pelo valor de DX --- */
  distanPer = (((pulseCountA + pulseCountB) / 2.0) * DX);
}

void updateBuzzer() {
  if ((millis() - timeBuzz) < 100) digitalWrite(BUZZ, HIGH);
  else digitalWrite(BUZZ, LOW);
}


int getError() {
  /* Condições esperadas de erro
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

  //verificaReta = 1;

  long valueSensores[7];
  erro = 0;

    /* --- Converte a informação analogica dos sensores em digital 
          de acordo com o limiar definido e da cor da linha --- */
    for (int i = 0; i < 7; i++) {
      if (analogRead(PIN_SENSORS[i]) > LIMIAR_SENSOR) valueSensores[i] = (COR_LINHA == 0) ? 1 : 0;
      else valueSensores[i] = (COR_LINHA == 0) ? 0 : 1;

      //Serial.print(String(analogRead(PIN_SENSORS[i]))+ ",");
    }
    //Serial.println();

    if      (valueSensores[1] == 1 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -4;
    else if (valueSensores[1] == 1 && valueSensores[2] == 1 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -3;
    else if (valueSensores[1] == 0 && valueSensores[2] == 1 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -2;
    else if (valueSensores[1] == 0 && valueSensores[2] == 1 && valueSensores[3] == 1 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -1;
    else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 1 && valueSensores[4] == 0 && valueSensores[5] == 0) erro =  0;
    else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 1 && valueSensores[4] == 1 && valueSensores[5] == 0) erro =  1;
    else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 1 && valueSensores[5] == 0) erro =  2;
    else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 1 && valueSensores[5] == 1) erro =  3;
    else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 1) erro =  4;
    else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = erroAnterior;

  return erro;
}

int getStateSensor(int value){
  int r = 0;
  if(value < 10) r = 0;
  else if(value >= 10 && value <= 90) r = 1;
  else r = 2;
  return r;
}

void setupPins() {           
  /* --- Setando pinos relacionados ao motor A --- */
  pinMode(PIN1_MOTOR_A, OUTPUT);
  pinMode(PIN2_MOTOR_A, OUTPUT);
  pinMode(PINPWM_MOTOR_A, OUTPUT);
  pinMode(PIN2_ENCODER_A, INPUT_PULLUP);

  /* --- Setando pinos relacionados ao motor B --- */
  pinMode(PIN1_MOTOR_B, OUTPUT);
  pinMode(PIN2_MOTOR_B, OUTPUT);
  pinMode(PINPWM_MOTOR_B, OUTPUT);
  pinMode(PIN2_ENCODER_B, INPUT_PULLUP);

  /* --- Setando pino do buzzer como saída --- */
  pinMode(BUZZ, OUTPUT);

  /* --- Setando pinos dos sensores como entrada --- */
  for (int i = 0; i < 7; i++)
    pinMode(PIN_SENSORS[i], INPUT_PULLUP);
  
  /* --- Habilitando interrupção para os pinos dos encoders --- */
  attachInterrupt(digitalPinToInterrupt(PIN1_ENCODER_A), interruptEncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN1_ENCODER_B), interruptEncoderB, RISING);
}
