/*------ PARTE PRINCIPAL DO CÓDIGO ------
 * Nessa parte que é feito toda a parte do 
 * PID.
 * 
 * Em caso de duvidas consute o arquivo
 * texto ou algum dos ministrantes do 
 * minicurso ;)
 */

#include "define.h"

float P = 0, I = 0, D = 0, erroAnterior = 0; // Aqui fica as variaveis que armazenaram os erros para o PID 
int auxBotao1 = 0; // Auxiliar para o botão 1, usado para tratar o entrada do botão
int pwm_A = 0, pwm_B = 0; // Valores do PWM que serão adicionado a velocidade das rodas
int timerBuzz = 0; // Auxiliar para o beep do buzzer

void setup() {
  
  Serial.begin(115200);
  
  // Definindo os pinos de Define
  // Botões
  pinMode(BOTAO1, INPUT_PULLUP);

  //Motor A
  pinMode(PIN1_MOTOR_A, OUTPUT);
  pinMode(PIN2_MOTOR_A, OUTPUT);
  pinMode(PINPWM_MOTOR_A, OUTPUT);

  //Motor B
  pinMode(PIN1_MOTOR_B, OUTPUT);
  pinMode(PIN2_MOTOR_B, OUTPUT);
  pinMode(PINPWM_MOTOR_B, OUTPUT);

  // Definindo os sensores como input
  for (int i = 0; i < 7; i++)
      pinMode(PIN_SENSORS[i], INPUT_PULLUP);

}

void loop() {
  
  // Bipe que ocorre quando o seguidor é ligado
  if ((millis() - timerBuzz) < 100) {
    digitalWrite(BUZZ, HIGH);
  }
  else digitalWrite(BUZZ, LOW);

  /* StatusUpdate() é responsavel por mudar o
   * estado do seguidor de parado para seguindo
   */
  statusUpdate();

  if (STATUS == SEGUINDO) { // Verifica se é verdadeira a condição de seguindo do sistema
    //float vel_A;
    //float vel_B;
    int maxPWM = 150; // Velocidade inicial do seguidor, velocidade base quando não à erro
    
    int PID = calculatePID(); // Função que calcula a o PID com base no erro apresentado




      digitalWrite(PIN1_MOTOR_A, LOW);
      digitalWrite(PIN2_MOTOR_A, HIGH);
      digitalWrite(PIN1_MOTOR_B, LOW);
      digitalWrite(PIN2_MOTOR_B, HIGH);
 
      //MOTOR A
      pwm_A = maxPWM  + PID;
      //MOTOR B
      pwm_B = maxPWM  - PID;
     
      analogWrite(PINPWM_MOTOR_A, pwm_A/1.5);
      analogWrite(PINPWM_MOTOR_B, pwm_B/1.5);

    /* Verifica-se no caso acima que para um motor o maxPWM e somado e
     *  no outro e sobtraido o valor do PID calculado.
     */

  }
}

// Procedimento do statusUpdate
void statusUpdate() {
  int b1 = digitalRead(BOTAO1);


  // BOTÃO 1 
  if (b1 == 0 && auxBotao1 == 0) {
    STATUS = SEGUINDO;
    Bipe();

    auxBotao1 = 1;
    delay(100);
  }
  else if (b1 == 1)
    auxBotao1 = 0;
}

// Função que calcula o PID
int calculatePID() {
  
  int acao; // Variavel que retorna o valor calculdo pelo PID
  int erro = getError(); // Chama a função para o tratamento dos erros
  
  P = erro; // Proporcional
  I = I + erro; // Integral
  D = erro - erroAnterior; // Derivativo

  // Limitando o derivativo para não ser superior a 255
  if(I > 255){
    I = 255;
  }

  // Calculo da ação do PID
  acao = (KP * P) + (KI * I) + (KD * D);

  // Atualização do erraAnterior
  erroAnterior = erro;

  // Retorna o valor da ação do PID
  return acao;
}

int getError() {
  /* Combinação de erros possiveis para o seguidor
   
     1 0 0 0 0    (-4)
     1 1 0 0 0    (-3)
     0 1 0 0 0    (-2)
     0 1 1 0 0    (-1)
     0 0 1 0 0    ( 0)
     0 0 1 1 0    ( 1)
     0 0 0 1 0    ( 2)
     0 0 0 1 1    ( 3)
     0 0 0 0 1    ( 4)
  */

  // Faz a leitura dos sensores
  int valueSensores[7], erro;

  /*  Discretiza o sinal do sensor com base no valor
   *  do limiar escolhido, associando com o tipo de
   *  faixa (trilha) escolhida.
   */
  for (int i = 0; i < 7; i++) {
    if (analogRead(PIN_SENSORS[i]) > LIMIAR_SENSOR)
      valueSensores[i] = (COR_LINHA == 0) ? 1 : 0;
    else
      valueSensores[i] = (COR_LINHA == 0) ? 0 : 1;

    //Serial.print(analogRead(PIN_SENSORS[i]));
    //Serial.print(" , ");
  }
  
  /*
   * Faz-se a associação do tipo de erro obtido a partir da leitura dos sensores  
   */
  if      (valueSensores[1] == 1 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -4;
  else if (valueSensores[1] == 1 && valueSensores[2] == 1 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -3;
  else if (valueSensores[1] == 0 && valueSensores[2] == 1 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -2;
  else if (valueSensores[1] == 0 && valueSensores[2] == 1 && valueSensores[3] == 1 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = -1;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 1 && valueSensores[4] == 0 && valueSensores[5] == 0) erro = 0;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 1 && valueSensores[4] == 1 && valueSensores[5] == 0) erro = 1;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 1 && valueSensores[5] == 0) erro = 2;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 1 && valueSensores[5] == 1) erro = 3;
  else if (valueSensores[1] == 0 && valueSensores[2] == 0 && valueSensores[3] == 0 && valueSensores[4] == 0 && valueSensores[5] == 1) erro = 4;
  
  return erro; // Retorna o erro o tipo de erro encontrado.
}

//Procedimento que realiza o beep
void Bipe() {
  timerBuzz = millis();
}
