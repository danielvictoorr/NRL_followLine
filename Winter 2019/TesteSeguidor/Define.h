#define SEGUINDO 1 //
#define PARADO 0   //
int STATUS = PARADO;

#define COR_LINHA 1
#define LIMIAR_SENSOR 890

#define PIN1_MOTOR_B 7
#define PIN2_MOTOR_B 8
#define PINPWM_MOTOR_B 6

#define PIN1_MOTOR_A 9
#define PIN2_MOTOR_A 10
#define PINPWM_MOTOR_A 5

#define BOTAO1 13
#define BOTAO2 12

#define KP 20
#define KI 0
#define KD 5

#define NUM_CRUZAMENTO 0
#define NUM_SEGMENTOS 16

int VEL_CURVA[NUM_SEGMENTOS] = {
                                80,
                                80,
                                80,
                                80,
                                80,
                                80,
                                150,
                                80,
                                80,
                                80,
                                80,
                                80,
                                80,
                                80,
                                80,
                                80,
                               
                               
                                                             
                             
  };

int PIN_SENSORS[7] = {A6, A0, A1, A2, A3, A4, A7};
