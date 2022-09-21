#include <Arduino.h>
#include <MotorEncoderNRL.h>
#include <SeguidorNRL.h>

#define STOPPED 0
#define FOLLOWING_LINE 1
#define NO_LINE 2

SeguidorNRL::SeguidorNRL(int pins[7], MotorEncoderNRL motorA, MotorEncoderNRL motorB, int lineMode) { // lineMode 1 para linha preta e 0 para linha branca
	for(int i=0; i<7; i++) {
		PinSensors[i] = pins[i];
		pinMode(PinSensors[i], INPUT);
	}

	MotorA = motorA;
	MotorA = motorB;

	this->lineMode = lineMode;
}

/*

	Sensor Array 	Error Value
	 0 0 0 0 1	 		4              
	 0 0 0 1 1			3              
	 0 0 0 1 0			2              
	 0 0 1 1 0			1              
	 0 0 1 0 0	 		0              
	 0 1 1 0 0		   -1              
	 0 1 0 0 0		   -2              
	 1 1 0 0 0		   -3              
	 1 0 0 0 0		   -4              

	 1 1 1 1 1          0   Robot found continuous line : STOPPED
	 0 0 0 0 0          0   Robot found no line: turn 180o

*/
int SeguidorNRL::readSensors() {
	int error = 0;
	int sensorValue[7];

	for(int i=0; i<7; i++)
		sensorValue[i] = analogRead(PinSensors[i]);

	for(int i=0; i<7; i++){
		if(lineMode == 1)
			if(sensorValue[i] < limiarSensor) sensorValue[i]=0;
			else sensorValue[i] = 1;
		else
			if(sensorValue[i] < limiarSensor) sensorValue[i]=1;
			else sensorValue[i] = 0;
	}

	if     ((sensorValue[1]== 0 )&&(sensorValue[2]== 0 )&&(sensorValue[3]== 0 )&&(sensorValue[4]== 0 )&&(sensorValue[5]== 1 ))  {mode = FOLLOWING_LINE; error = 4;}
	else if((sensorValue[1]== 0 )&&(sensorValue[2]== 0 )&&(sensorValue[3]== 0 )&&(sensorValue[4]== 1 )&&(sensorValue[5]== 1 ))  {mode = FOLLOWING_LINE; error = 3;}
	else if((sensorValue[1]== 0 )&&(sensorValue[2]== 0 )&&(sensorValue[3]== 0 )&&(sensorValue[4]== 1 )&&(sensorValue[5]== 0 ))  {mode = FOLLOWING_LINE; error = 2;}
	else if((sensorValue[1]== 0 )&&(sensorValue[2]== 0 )&&(sensorValue[3]== 1 )&&(sensorValue[4]== 1 )&&(sensorValue[5]== 0 ))  {mode = FOLLOWING_LINE; error = 1;}
	else if((sensorValue[1]== 0 )&&(sensorValue[2]== 0 )&&(sensorValue[3]== 1 )&&(sensorValue[4]== 0 )&&(sensorValue[5]== 0 ))  {mode = FOLLOWING_LINE; error = 0;}
	else if((sensorValue[1]== 0 )&&(sensorValue[2]== 1 )&&(sensorValue[3]== 1 )&&(sensorValue[4]== 0 )&&(sensorValue[5]== 0 ))  {mode = FOLLOWING_LINE; error = -1;}
	else if((sensorValue[1]== 0 )&&(sensorValue[2]== 1 )&&(sensorValue[3]== 0 )&&(sensorValue[4]== 0 )&&(sensorValue[5]== 0 ))  {mode = FOLLOWING_LINE; error = -2;}
	else if((sensorValue[1]== 1 )&&(sensorValue[2]== 1 )&&(sensorValue[3]== 0 )&&(sensorValue[4]== 0 )&&(sensorValue[5]== 0 ))  {mode = FOLLOWING_LINE; error = -3;}
	else if((sensorValue[1]== 1 )&&(sensorValue[2]== 0 )&&(sensorValue[3]== 0 )&&(sensorValue[4]== 0 )&&(sensorValue[5]== 0 ))  {mode = FOLLOWING_LINE; error = -4;}
	else if((sensorValue[1]== 1 )&&(sensorValue[2]== 1 )&&(sensorValue[3]== 1 )&&(sensorValue[4]== 1 )&&(sensorValue[5]== 1 ))  {mode = STOPPED; error = 0;}
	else if((sensorValue[1]== 0 )&&(sensorValue[2]== 0 )&&(sensorValue[3]== 0 )&&(sensorValue[4]== 0 )&&(sensorValue[5]== 0 ))  {mode = NO_LINE; error = 0;}

	erro = error;

	return error;
}

void SeguidorNRL::printError() {
	Serial.print("Error: ");
	Serial.println(erro);
}

void SeguidorNRL::motorControl() {
	float PIDvalue = calculatePID();
	int velMotorA = velNominal + PIDvalue;
	int velMotorB = velNominal - PIDvalue;

	//velMotorA = map(velMotorA, -73, 327, 0, 200);
	//velMotorB = map(velMotorB, -73, 327, 0, 200);

	MotorA.setVel(velMotorA);
	MotorB.setVel(velMotorB);

	MotorA.updateVel();
	MotorB.updateVel();

	Serial.print (PIDvalue);
	Serial.print (" --> Left, Right:   ");
	Serial.print (velMotorA);
	Serial.print ("   ");
	Serial.println (velMotorB);
}

float SeguidorNRL::calculatePID() {
	readSensors();

	float P = erro;
	float I = I + erro;
	float D = erro - erroAnterior;

	float PID = (Kp*P) + (Ki*I) + (Kd*D);

	erroAnterior = erro;
	return PID;
}

void SeguidorNRL::checkPIDvalues() {
	Serial.print("PID: ");
	Serial.print(Kp);
	Serial.print(" - ");
	Serial.print(Ki);
	Serial.print(" - ");
	Serial.println(Kd);  
}

void SeguidorNRL::checkSensors(int analogOrDigital) {
	int sensorValue[7];

	for(int i=0; i<7; i++)
		sensorValue[i] = analogRead(PinSensors[i]);

	if(analogOrDigital == 0)
		for(int i=0; i<7; i++){
			if(sensorValue[i] < limiarSensor) sensorValue[i] = 0;
			else sensorValue[i] = 1;
		}
	
	Serial.print("Sensor values: ");
	for(int i=0; i<7; i++){
		Serial.print(sensorValue[i]);
		Serial.print(" ");
	}
}

void SeguidorNRL::setPID(float P, float I, float D) {
	Kp = P;
	Ki = I;
	Kd = D;
}

void SeguidorNRL::setLimiarSensor(int l) {
	limiarSensor = l;
}

void SeguidorNRL::setVelNominal(int v) {
	velNominal = v;
}

