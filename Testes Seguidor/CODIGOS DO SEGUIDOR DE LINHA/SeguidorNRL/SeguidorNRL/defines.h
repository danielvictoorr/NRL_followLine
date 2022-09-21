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

#define LIMIAR_SENSOR 600
int PIN_SENSORS[7]={A0,A1,A2,A3,A4,A5,A6};

int VEL_MAX = 2000;

#define KP 600
#define KI 150
#define KD 600

#define DIST_PULSO (37 * PI)/15

#define NUM_CURVA 5
int VEL_CURVA[(NUM_CURVA*2)+1] = { 2000,  // 1
							     600,  // 2
						         1500,  // 3
							     1500,  // 4
						  	     1500,  // 5
							     1000,  // 6
							     1000,  // 7
							     1000,  // 8
							     1500,
							     800,  // 9
							       0 // 10
							    
};