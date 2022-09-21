//#include <MotorEncoderNRL.h>
#include "defines.h"
#include "MotorCtrl.h"
//#include "NADA.h"

void interupA();
void interupB();

MotorCtrl motorA(PIN2_MOTOR_A, PIN1_MOTOR_A, PINPWM_MOTOR_A, ENCODER1_MOTOR_A, -1, ((37 * PI) / 15), interupA);
MotorCtrl motorB(PIN1_MOTOR_B, PIN2_MOTOR_B, PINPWM_MOTOR_B, ENCODER1_MOTOR_B, -1, ((37 * PI) / 15), interupB);

int erro = 0;
int erroAnterior = 0;
int indexCurva = 0, auxCurva = 0, indexInicioFim = 0, auxInicioFim = 0 ;
int auxBotao1 = 0;

unsigned long tempo=millis();

void setup() {
	Serial.begin(115200);
	Serial.println("Setup");
 	setupPins();
 	motorA.setKP(1);
  	motorB.setKP(1);
}

void loop() {
	Serial.println(millis()-tempo);
	tempo=millis();
	/*motorA.setSpeed(-800);
	motorB.setSpeed(-800);
	Serial.println(motorA.getVel());
	motorA.updateCtrl();
  	motorB.updateCtrl();*/
	
	atualizaStatus();
	
    if(STATUS == SEGUINDO){
		//MA.updateVel();
		//MB.updateVel();
		
		//Serial.println(MA.encod.getDist());
		//delay(50);
		int acao = calculatePID();
		//MA.vel_ref = VEL_MAX + acao;
		//MB.vel_ref = VEL_MAX - acao;
		int speedA = VEL_MAX + acao;
		int speedB = VEL_MAX - acao;
		/*
		if(speedA < 0)
			speedA=0;
		if(speedB < 0)
			speedB=0;*/
		motorA.setSpeed(speedA);
		motorB.setSpeed(speedB);

		/*Serial.print(speedA);
		Serial.print(",");
		Serial.println(speedB);*/

		Serial.println(acao);

		identificaCurva();
		//inicio_fim(); // <------- DESCOMENTAR PRA PROVA!!

		//Serial.println(digitalRead(BOTAO1));

		motorA.updateCtrl();
  		motorB.updateCtrl();

  		//ajusteVelocidade();
	}
}

void freiaMotor(){
	if(isInArray(indexCurva, CURVA_PARADA)==0){
		Serial.println("Freia");
		digitalWrite(PIN2_MOTOR_A, LOW);
		digitalWrite(PIN1_MOTOR_A, HIGH);
		analogWrite(PINPWM_MOTOR_A, 100);

		digitalWrite(PIN1_MOTOR_B, LOW);
		digitalWrite(PIN2_MOTOR_B, HIGH);
		analogWrite(PINPWM_MOTOR_B, 100);
		
		delay(100);
	}
}

int isInArray(int val, int array[]){
	int r = 0;
	for(int i=0; i<24; i++){
		if(array[i]==val)
			r=1;
	}
	return r;
}

void atualizaStatus() {
	int b1 = digitalRead(BOTAO1);
	if (b1 == 0 && auxBotao1 == 0) {
		delay(500);
		STATUS = SEGUINDO;
    	auxBotao1 = 1;
    	//Serial.println(indexCurva);
    	//Serial.println("comecou");
    }
    else if (b1 == 1)
    	auxBotao1 = 0;
}

void(* resetFunc) (void) = 0;

void ajusteVelocidade() {
	VEL_MAX = VEL_CURVA[indexCurva];  // <------- TROCAR PRA i !!
}

void inicio_fim() {
	int valueSensor = 0;
	if(analogRead(PIN_SENSORS[6]) > LIMIAR_SENSOR) valueSensor = (COR_LINHA == 0)? 1:0;
	else valueSensor = (COR_LINHA == 0)? 0:1;

    if (valueSensor == 1 && auxInicioFim == 0) {
        indexInicioFim = indexInicioFim + 1;
    	//Serial.println(indexCurva);
    	if(indexInicioFim == (NUM_CRUZAMENTO + 2)){
			STATUS = PARADO;
			//MA.vel_ref = 0;
			//MB.vel_ref = 0;
			motorA.setSpeed(0);
			motorB.setSpeed(0);
			//MA.updateVel();
			//MB.updateVel();
			motorA.updateCtrl();
  			motorB.updateCtrl();
			resetFunc();  
    	}

    	Serial.println(indexInicioFim);

    	auxInicioFim = 1;
    }
    else if (valueSensor == 0)
    	auxInicioFim = 0;
}

void identificaCurva() {
	int valueSensor = 0;
	if(analogRead(PIN_SENSORS[0]) > LIMIAR_SENSOR) valueSensor = (COR_LINHA == 0)? 1:0;
	else valueSensor = (COR_LINHA == 0)? 0:1;
    if (valueSensor == 1 && auxCurva == 0) {
    	indexCurva++;
    	auxCurva = 1;
    	freiaMotor();
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

	for(int i=0; i<7; i++){
		//Serial.print(analogRead(PIN_SENSORS[i]));
		//Serial.print(",");
		if(analogRead(PIN_SENSORS[i]) > LIMIAR_SENSOR) valueSensores[i] = (COR_LINHA == 0)? 1:0;
		else valueSensores[i] = (COR_LINHA == 0)? 0:1;

		//Serial.print(valueSensores[i]);
		//Serial.print(",");

	}
	//Serial.println("");

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
	pinMode(BOTAO1, INPUT_PULLUP);
	pinMode(BOTAO2, INPUT_PULLUP);

	for(int i=0;i<7;i++)
		pinMode(PIN_SENSORS[i], INPUT);

	//attachInterrupt(digitalPinToInterrupt(ENCODER1_MOTOR_A), pulsoContA, RISING);
	//attachInterrupt(digitalPinToInterrupt(ENCODER1_MOTOR_B), pulsoContB, RISING);

	//MA.setupPins(PIN1_MOTOR_A, PIN2_MOTOR_A, PINPWM_MOTOR_A);
	//MB.setupPins(PIN1_MOTOR_B, PIN2_MOTOR_B, PINPWM_MOTOR_B);
}
void interupA() {
  motorA.encoderPulse();
}
void interupB() {
  motorB.encoderPulse();
}