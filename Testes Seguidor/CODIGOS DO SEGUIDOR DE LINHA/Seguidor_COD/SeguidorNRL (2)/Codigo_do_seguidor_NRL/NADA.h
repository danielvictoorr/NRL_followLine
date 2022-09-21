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

#define LIMIAR_SENSOR 600
int PIN_SENSORS[7]={A0,A1,A2,A3,A4,A5,A6};

int VEL_MAX = 2000;

#define KP 600
#define KI 150
#define KD 600

#define DIST_PULSO (42 * PI)/15

#define NUM_CRUZAMENTO 14
#define NUM_SEGUIMENTOS 51
int VEL_CURVA[NUM_SEGUIMENTOS] = { 200,  // 0
							       200,   // 1
						           200,  // 2
							       200,  // 3
						  	       200,  // 4
							       200,  // 5
							       200,  // 6
							       200,   // 7
							       200,  // 8
							       200,   // 9
							       200,   // 10
							       200,   // 11
							       200,  //12 
							       200,  //13
							       200,  //14
							       200,  //15
							       200,     //16
							       200,     //17
							       200,     //18
							       200,    //19
							       200,    //20
							       200,    //21
							       200,    //22
							       200,    //23
							       200,     //24
							       200,     //25
							       200,     //26
							       200,	//27
							       200,		//28
							       200,		//29
							       200,		//30
							       200,	//31
							       200,		//32
							       200, 	//33
							       200,	//34
							       200,	//35
							       200,	//36
							       200,	//37
							       200,     //38
							       200,		//39
							       200,		//40
							       200,		//41
							       200,		//42
							       200,    //43
							       200,    //44
							       200,     //45
							       200,		//46
							       200,		//47
							       200,	//48
							       200,		//49
							       200		//50
							      





};