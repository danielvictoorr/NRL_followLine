/*------ Código fonte para o minicurso Seguidor de linha ------ 
 *  Esse código é a sintese do que foi desenvolvido no minicurso
 * do seguidor de linha ministrado pela equipe do NRL.
 */
 
#define SEGUINDO 1 // Estado no qual o seguidor está se movendo
#define PARADO 0   // Estado no qual o seguidor está parado
int STATUS = PARADO; // Estado inicial do sistema

#define COR_LINHA 1 // 0 para linha preta e 1 para linha branca
#define LIMIAR_SENSOR 890

/*------ Definição dos pinos do seguidor ------
 *  Essa parte do código define quais são os pinos 
 * do arduino que serão usado pela Ponte H, assim
 * como quais os pinos associados aos botões que 
 * o seguidor tem. 
 * As definições desses pinos depende da Ponte H
 * usada.
 */

#define PIN1_MOTOR_B 7
#define PIN2_MOTOR_B 8
#define PINPWM_MOTOR_B 6

#define PIN1_MOTOR_A 9
#define PIN2_MOTOR_A 10
#define PINPWM_MOTOR_A 5

#define BOTAO1 13
#define BUZZ A5

// Pinos dos sensores QRE 1113.
int PIN_SENSORS[7] = {A6, A0, A1, A2, A3, A4, A7};

/*------------ Ganhos do PID ------------
 * Os ganhos do PID são valores escolhisdos
 * por tentativa é erro.
 */

#define KP 35 // Constante propocional
#define KI 0 // Constante integrativa
#define KD 11 // Constante derivativa
























































 
