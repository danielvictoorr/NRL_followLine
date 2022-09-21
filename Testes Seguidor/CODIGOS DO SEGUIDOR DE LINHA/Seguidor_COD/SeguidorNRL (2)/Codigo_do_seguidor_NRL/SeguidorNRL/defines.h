#define COR_LINHA 1 // 0 para linha preta e 1 para linha branca

#define SEGUINDO 1 //
#define PARADO 0 //
int STATUS = PARADO;


#define PIN1_MOTOR_A 9
#define PIN2_MOTOR_A 8
#define PINPWM_MOTOR_A 10
#define ENCODER1_MOTOR_A 3
#define ENCODER2_MOTOR_A 3
#define PIN1_MOTOR_B 7
#define PIN2_MOTOR_B 6
#define PINPWM_MOTOR_B 5
#define ENCODER1_MOTOR_B 2
#define ENCODER2_MOTOR_B 2

#define BOTAO1 12
#define BOTAO2 11

#define LIMIAR_SENSOR 750
int PIN_SENSORS[7]={A0,A1,A2,A3,A4,A5,A6};

int VEL_MAX = 2000;

#define KP 400
#define KI 1
#define KD 600

#define DIST_PULSO (42 * PI)/15

#define NUM_CRUZAMENTO 14
#define NUM_SEGMENTOS 51
int VEL_CURVA[(NUM_SEGMENTOS)] = { 1500,  // 0
							       1000,   // 1
						           1000,  // 2
							       1000,  // 3
						  	       1000,  // 4
							       1000,  // 5
							       1000,  // 6
							       1000,   // 7
							       1000,  // 8
							       800,   // 9
							       1000,   // 10
							       1000,   // 11
							       1500,  //12 
							       1500,  //13
							       1000,  //14
							       1000,  //15
							       1500,     //16
							       1500,     //17
							       1000,     //18
							       1000,    //19
							       1500,    //20
							       1500,    //21
							       1000,    //22
							       800,    //23
							       1500,     //24
							       1500,     //25
							       1500,     //26
							       1000,	//27
							       1000,		//28
							       1000,		//29
							       1000,		//30
							       1000,	//31
							       1000,		//32
							       2000, 	//33
							       2000,	//34
							       2000,	//35
							       1500,	//36
							       1000,	//37
							       1000,     //38
							       1000,		//39
							       1000,		//40
							       1000,		//41
							       1500,		//42
							       1000,    //43
							       1000,    //44
							       1000,     //45
							       1000,		//46
							       1000,		//47
							       1000,	//48
							       1000,		//49
							       0		//50
};