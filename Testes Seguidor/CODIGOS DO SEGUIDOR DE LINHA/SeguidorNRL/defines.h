
#define COR_LINHA 0 // 0 para linha preta e 1 para linha branca

#define SEGUINDO 1 //
#define PARADO 0 //
int STATUS = PARADO;

#define PIN1_MOTOR_A 8
#define PIN2_MOTOR_A 9
#define PINPWM_MOTOR_A 5
#define ENCODER1_MOTOR_A 3
#define ENCODER2_MOTOR_A 3

#define PIN1_MOTOR_B 7
#define PIN2_MOTOR_B 6
#define PINPWM_MOTOR_B 10
#define ENCODER1_MOTOR_B 2
#define ENCODER2_MOTOR_B 2

#define BOTAO1 12
#define BOTAO2 11

#define LIMIAR_SENSOR 700
int PIN_SENSORS[7]={A0,A1,A2,A3,A4,A5,A6};

int VEL_MAX = 2000;

#define KP 180
#define KI 0.1
#define KD 20

#define DIST_PULSO (42 * PI)/15

int CURVA_PARADA[24]={9,10,12,13,14,16,17,18,20,21,22,24,25,26,27,31,33,34,35,36,37,43,44,50};

#define NUM_CRUZAMENTO 1
#define NUM_SEGUIMENTOS 10
int VEL_CURVA[NUM_SEGUIMENTOS] = { 
	300,
	300,
	300,
	300,
	300,
	300,
	300,
	300,
	300,
	300							      
};