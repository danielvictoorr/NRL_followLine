
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

#define KP 800
#define KI 0
#define KD 0

#define DIST_PULSO (42 * PI)/15

#define NUM_CRUZAMENTO 14
#define NUM_SEGUIMENTOS 51
int VEL_CURVA[NUM_SEGUIMENTOS] = { 2000,  // 0
							       1000,   // 1
						           1000,  // 2
							       1000,  // 3
						  	       1000,  // 4
							       1000,  // 5
							       1000,  // 6
							       1000,   // 7
							       1500,  // 8
							       1500,   // 9
							       1500,   // 10
							       1000,   // 11
							       2000,  //12 
							       2000,  //13
							       1500,  //14
							       1000,  //15
							       1500,     //16
							       1500,     //17
							       1500,     //18
							       1000,    //19
							       2000,    //20
							       2000,    //21
							       1500,    //22
							       1000,    //23
							       2000,     //24
							       2000,     //25
							       2000,     //26
							       1000,	//27
							       1000,		//28
							       1000,		//29
							       1000,		//30
							       1500,	//31
							       1000,		//32
							       2000, 	//33
							       2500,	//34
							       2500,	//35
							       2000,	//36
							       1000,	//37
							       1000,     //38
							       1000,		//39
							       1000,		//40
							       1000,		//41
							       1000,		//42
							       1500,    //43
							       1500,    //44
							       1000,     //45
							       1500,		//46
							       1000,		//47
							       1500,	//48
							       1000,		//49
							       2000		//50
							      
};