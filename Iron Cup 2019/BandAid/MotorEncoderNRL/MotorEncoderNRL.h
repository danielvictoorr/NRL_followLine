#ifndef MotorEncoderNRL_h_
#define MotorEncoderNRL_h_

#include <Arduino.h>

class MotorEncoderNRL {

private:
	int Pin1;
	int Pin2;
	int PinPWM;
	int PinEncoder1;
	int PinEncoder2;
	int DiametroRoda;
	int PulsosPorVolta;

	int TimeOut = 500;

	int Velocidade = 0;

	float Kp = 1;

	int pulsos = 0;
	unsigned int pulso_time = 0; //Em ms
	unsigned int pulso_dt = 0; //Em ms

public:
	MotorEncoderNRL(int pin1, int pin2, int pinPWM, int pinEncoder1, int pinEncoder2, int diametroRoda, int pulsosPorVolta);
	void setVel(int vel);
	void setTimeOut(int time);
	float getDistPorPulso();
	int getVel();
	void updateVel();
	void pulsoCont();
	void setP(float p);
};

#endif