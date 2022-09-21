
#define COR_LINHA 1 // 0 para linha preta e 1 para linha branca

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

#define KP 550
#define KI 0.1
#define KD 300

#define DIST_PULSO (42 * PI)/15

int CURVA_PARADA[24]={9,10,12,13,14,16,17,18,20,21,22,24,25,26,27,31,33,34,35,36,37,43,44,50};

#define NUM_CRUZAMENTO 14
#define NUM_SEGUIMENTOS 51
int VEL_CURVA[NUM_SEGUIMENTOS] = { 1000,  // 0
							       600,   // 1
						           600,  // 2
							       600,  // 3
						  	       600,  // 4
							       600,  // 5
							       600,  // 6
							       900,   // 7
							       1000,  // 8
							       800,   // 9
							       800,   // 10
							       600,   // 11
							       800,  //12 
							       800,  //13
							       800,  //14
							       1000,  //15
							       1200,     //16
							       1000,     //17
							       800,     //18
							       800,    //19
							       1000,    //20
							       1200,    //21
							       800,    //22
							       900,    //23
							       1200,     //24
							       1000,     //25
							       1000,     //26
							       800,	//27------
							       800,		//28
							       1000,		//29
							       900,		//30
							       1000,	//31
							       800,		//32
							       1200, 	//33
							       1000,	//34
							       1200,	//35
							       1000,	//36
							       1000,	//37
							       900,     //38
							       1000,		//39
							       900,		//40
							       1000,		//41
							       900,		//42
							       1000,    //43
							       900,    //44
							       800,     //45
							       1000,		//46
							       900,		//47
							       1000,	//48
							       800,		//49
							       0		//50
							      
};