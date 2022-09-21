#include <Arduino.h>
#include <MotorEncoderNRL.h>

MotorEncoderNRL::MotorEncoderNRL(int pin1, int pin2, int pinPWM, int pinEncoder1, int pinEncoder2, int diametroRoda, int pulsosPorVolta) {
	Pin1 = pin1;
	Pin2 = pin2;
	PinPWM = pinPWM;
	PinEncoder1 = pinEncoder1;
	PinEncoder2 = pinEncoder2;
	DiametroRoda = diametroRoda;
	PulsosPorVolta = pulsosPorVolta;

	pinMode(Pin1, OUTPUT);
	pinMode(Pin2, OUTPUT);
	pinMode(PinPWM, OUTPUT);

	digitalWrite(Pin1, LOW);
	digitalWrite(Pin2, LOW);
	digitalWrite(PinPWM, LOW);
}

void MotorEncoderNRL::setVel(int vel) {
	Velocidade = vel;
}

void MotorEncoderNRL::setTimeOut(int time) {
	TimeOut = time;
}


float MotorEncoderNRL::getDistPorPulso() {
	return (DiametroRoda * PI) / PulsosPorVolta;
}

int MotorEncoderNRL::getVel() {
	if (pulso_dt <= 0  || (micros() - pulso_time) > TimeOut)
		return 0;
	return ((1000L * getDistPorPulso() / (pulso_dt / 1000L)) );
}

void MotorEncoderNRL::updateVel() {
	//digitalWrite(PINMOTOR,vel_ref);
	long erro = Velocidade - getVel();
	long acao = Kp * erro;

	//Serial.println(acao);

	//Zona morta
	if (abs(acao) >= 30) {
		//acao += 30;
		if (acao > 0) { // Inverter Ponte H
			digitalWrite(Pin1, HIGH);
			digitalWrite(Pin2, LOW);
		}
		else {
			acao = -acao;
			digitalWrite(Pin1, LOW);
			digitalWrite(Pin2, LOW);
		}
		//Saturação
		acao = acao > 200 ? 200 : acao;
		analogWrite(PinPWM, acao);
	}
	else {
		digitalWrite(Pin1, LOW);
		digitalWrite(Pin2, LOW);
		digitalWrite(PinPWM, 0);
	}
 }

void MotorEncoderNRL::pulsoCont() {
	pulso_dt =  micros() - pulso_time;
	pulso_time = micros();
	pulsos++;
}

void MotorEncoderNRL::setP(float p){
	Kp = p;
}
