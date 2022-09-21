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

#define LIMIAR_SENSOR 750
int PIN_SENSORS[7]={A0,A1,A2,A3,A4,A5,A6};

int VEL_MAX = 2000;

#define KP 600
#define KI 150
#define KD 600

#define DIST_PULSO (42 * PI)/15

#define NUM_CRUZAMENTO 14
#define NUM_SEGMENTOS 51
int VEL_CURVA[(NUM_SEGMENTOS)] = { 1000,  // 0
							       600,   // 1
						           600,  // 2
							       600,  // 3
						  	       600,  // 4
							       600,  // 5
							       600,  // 6
							       800,   // 7
							       800,  // 8
							       1000,   // 9
							       600,   // 10
							       1000,   // 11
							       1000,  //12 
							       1000,  //13
							       800,  //14
							       1000,  //15
							       1000,     //16
							       1000,     //17
							       800,     //18
							       1000,    //19
							       1000,    //20
							       1000,    //21
							       800,    //22
							       1000,    //23
							       1000,     //24
							       1000,     //25
							       600,     //26
							       600,	//27
							       800,		//28
							       800,		//29
							       600,		//30
							       1000,	//31
							       1000,		//32
							       1000, 	//33
							       1000,	//34
							       800,	//35
							       600,	//36
							       600,	//37
							       600,     //38
							       1000,		//39
							       1000,		//40
							       600,		//41
							       600,		//42
							       1000,    //43
							       600,    //44
							       1000,     //45
							       200,		//46
							       200,		//47
							       200,	//48
							       200,		//49
							       200		//50
};