//#include <MotorEncoderNRL.h>
#include "defines.h"
#include "NADA.h"

int erro = 0;
int erroAnterior = 0;
int indexCurva = 0, auxCurva = 0, marcador = 0 ;

void setup() {
	Serial.begin(115200);
	Serial.println("Setup");
 	setupPins();
}

void loop() {
	MA.updateVel();
	MB.updateVel();
	//Serial.println(MA.encod.getDist());
	//delay(50);
	int acao = calculatePID();
	MA.vel_ref = VEL_MAX + acao;
	MB.vel_ref = VEL_MAX - acao;

	identificaCurva();
	//inicio_fim();
}
void ajusteVelocidade(){
	VEL_MAX = VEL_CURVA[indexCurva];
}

void inicio_fim(){
	int valueSensor = 0;
	if(analogRead(PIN_SENSORS[7]) > LIMIAR_SENSOR) valueSensor = 1;
	else valueSensor = 0;
    if (valueSensor == 1 && auxCurva == 0) {
        marcador = marcador + 1;
    	auxCurva = 1;
    	//Serial.println(indexCurva);
    	if(marcador == 2){
    		MA.vel_ref = 0;
			MB.vel_ref = 0;
    	}
    }
    else if (valueSensor == 0)
    	auxCurva = 0;
}

void identificaCurva(){
	int valueSensor = 0;
	if(analogRead(PIN_SENSORS[0]) > LIMIAR_SENSOR) valueSensor = 1;
	else valueSensor = 0;
    if (valueSensor == 1 && auxCurva == 0) {
    	indexCurva = indexCurva + 1;
    	auxCurva = 1;
    	//Serial.println(indexCurva);
    }
    else if (valueSensor == 0)
    	auxCurva = 0;

    ajusteVelocidade();
}

int calculatePID() {
	int acao, erro = getError();
    int P = erro;
    int I = I + erro;
    int D = erro - erroAnterior;
    acao = (KP*P) + (KI*I) + (KD*D);
	//acao = erro*KP;
    erroAnterior = erro;
	return acao;
}


int getError() {
		/*
		* 1 0 0 0 0
		* 1 1 0 0 0
		* 0 1 0 0 0
		* 0 1 1 0 0
		* 0 0 1 0 0
		* 0 0 1 1 0
		* 0 0 0 1 0
		* 0 0 0 1 1
		* 0 0 0 0 1
		*/

	int valueSensores[7];

	for(int i=0; i<7; i++)
	if(analogRead(PIN_SENSORS[i]) > LIMIAR_SENSOR) valueSensores[i] = 1;
	else valueSensores[i] = 0;

	if     (valueSensores[1]==1 && valueSensores[2]==0 && valueSensores[3]==0 && valueSensores[4]==0 && valueSensores[5]==0) erro = -4;
	else if(valueSensores[1]==1 && valueSensores[2]==1 && valueSensores[3]==0 && valueSensores[4]==0 && valueSensores[5]==0) erro = -3;
	else if(valueSensores[1]==0 && valueSensores[2]==1 && valueSensores[3]==0 && valueSensores[4]==0 && valueSensores[5]==0) erro = -2;
	else if(valueSensores[1]==0 && valueSensores[2]==1 && valueSensores[3]==1 && valueSensores[4]==0 && valueSensores[5]==0) erro = -1;
	else if(valueSensores[1]==0 && valueSensores[2]==0 && valueSensores[3]==1 && valueSensores[4]==0 && valueSensores[5]==0) erro =  0;
	else if(valueSensores[1]==0 && valueSensores[2]==0 && valueSensores[3]==1 && valueSensores[4]==1 && valueSensores[5]==0) erro =  1;
	else if(valueSensores[1]==0 && valueSensores[2]==0 && valueSensores[3]==0 && valueSensores[4]==1 && valueSensores[5]==0) erro =  2;
	else if(valueSensores[1]==0 && valueSensores[2]==0 && valueSensores[3]==0 && valueSensores[4]==1 && valueSensores[5]==1) erro =  3;
	else if(valueSensores[1]==0 && valueSensores[2]==0 && valueSensores[3]==0 && valueSensores[4]==0 && valueSensores[5]==1) erro =  4;

	return erro;
}

void setupPins() {
	pinMode(PIN1_MOTOR_A, OUTPUT);
	pinMode(PIN2_MOTOR_A, OUTPUT);
	pinMode(PINPWM_MOTOR_A, OUTPUT);
	pinMode(ENCODER1_MOTOR_A, INPUT);
	pinMode(ENCODER2_MOTOR_A, INPUT);
	pinMode(PIN1_MOTOR_B, OUTPUT);
	pinMode(PIN2_MOTOR_B, OUTPUT);
	pinMode(PINPWM_MOTOR_B, OUTPUT);
	pinMode(ENCODER1_MOTOR_B, INPUT);
	pinMode(ENCODER2_MOTOR_B, INPUT);

	for(int i=0;i<7;i++)
		pinMode(PIN_SENSORS[i], INPUT);

	attachInterrupt(digitalPinToInterrupt(ENCODER1_MOTOR_A), pulsoContA, RISING);
	attachInterrupt(digitalPinToInterrupt(ENCODER1_MOTOR_B), pulsoContB, RISING);

	MA.setupPins(PIN1_MOTOR_A, PIN2_MOTOR_A, PINPWM_MOTOR_A);
	MB.setupPins(PIN1_MOTOR_B, PIN2_MOTOR_B, PINPWM_MOTOR_B);
}

