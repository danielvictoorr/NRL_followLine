#ifndef SeguidorNRL_h_
#define SeguidorNRL_h_

#include <Arduino.h>
#include <MotorEncoderNRL.h>

class SeguidorNRL {

private:
	int PinSensors[7];
	MotorEncoderNRL MotorA = MotorEncoderNRL(0,0,0,0,0,0,0);
	MotorEncoderNRL MotorB = MotorEncoderNRL(0,0,0,0,0,0,0);

	int velNominal = 1000;

	int mode;
	int erro;
	int erroAnterior;

	float Kp = 0;
	float Ki = 0;
	float Kd = 0;

	int lineMode = 0;

	int limiarSensor = 600;

public:
	SeguidorNRL(int pins[7], MotorEncoderNRL motorA, MotorEncoderNRL motorB, int lineMode);
	void printError();
	int readSensors();
	void motorControl();
	float calculatePID();
	void checkPIDvalues();
	void checkSensors(int analogOrDigital);
	void setPID(float P, float I, float D);
	void setLimiarSensor(int l);
	void setVelNominal(int v);
};

#endif