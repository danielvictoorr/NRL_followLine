#include <Arduino.h>
#include "MotorCtrl.h"

#define FRENTE true
#define TRAS false

MotorCtrl::PonteH::PonteH(byte pin_enableA, byte pin_enableB, byte pin_PWM)
{
    this->pin_enableA = pin_enableA;
    this->pin_enableB = pin_enableB;
    this->pin_PWM = pin_PWM;
    pinMode(pin_enableA, OUTPUT);
    pinMode(pin_enableB, OUTPUT);
    pinMode(pin_PWM, OUTPUT);
}

MotorCtrl::PonteH::PonteH()
{
}

void MotorCtrl::PonteH::setPot(int p)
{
    //Serial.println(p);
    //Zona morta
    if (abs(p) >= deadZone_PWM)
    {
        if (p > 0)
        { // Inverter Ponte H
            //p=0;
            digitalWrite(pin_enableA, HIGH);
            digitalWrite(pin_enableB, LOW);
        }
        else
        {
            p = -p;
            //Serial.println(p);
            digitalWrite(pin_enableA, LOW);
            digitalWrite(pin_enableB, LOW);
        }
        //Saturação
        p = p > 255 ? 255 : p;
        analogWrite(pin_PWM, p);
    }
    else
    {
        digitalWrite(pin_enableA, LOW);
        digitalWrite(pin_enableB, LOW);
        digitalWrite(pin_PWM, 0);
    }
}

void MotorCtrl::PonteH::setDeadZone(byte deadZone)
{
    this->deadZone_PWM = deadZone;
}

MotorCtrl::Encoder::Encoder(byte pin_sinalA, byte pin_sinalB, int dist_p_pulse, void (*f)())
{

    this->pin_sinalA = pin_sinalA;
    this->dist_p_pulse = dist_p_pulse;
    this->pin_sinalB = pin_sinalB;
    attachInterrupt(digitalPinToInterrupt(pin_sinalA), f, RISING);
    //
    pinMode(pin_sinalA, INPUT_PULLUP);
    if (pin_sinalB != -1)
    {
        typeAB = true;
        pinMode(pin_sinalB, INPUT_PULLUP);
    }
    else
    {
        typeAB = false;
    }
    lastTime[0] = 0;
    lastTime[1] = 0;
    lastTime[2] = 0;
}

MotorCtrl::Encoder::Encoder(byte pin_encoderA, int dist_p_pulse, void (*f)())
{
    Encoder(pin_encoderA, -1, dist_p_pulse, f);
    vel=0;
}

MotorCtrl::Encoder::Encoder()
{
}

void MotorCtrl::Encoder::pulseEncoderA()
{
    bool direction = FRENTE; //inicia com direcao para frente

    if (!typeAB)                                               //Se encoder for tipo AB
        direction = (digitalRead(pin_sinalB)) ? FRENTE : TRAS; //Sinal
                                                               //caso o enconder esteja invertido inverter definições de FRENTE e TRAS

    unsigned long lt = lastTime[0] / 3 + lastTime[1] / 3 + lastTime[2] / 3;
    if (micros()-lt < 500000)
        vel = (double)dist_p_pulse * (1000000.0 / (micros() - lt)); //1000000L transforma microsegundos em segundos
    else
        vel = 0;

    position++;
    // if (direction)
    //     ;
    // else
    // {
    //     vel *= -1;
    //     position -= dist_p_pulse;
    // }

    //Serial.println(vel,DEC);
    // Serial.print(',');
    // Serial.println(0);

    lastTime[pulses % 3] = micros();
    pulses++;
}

long MotorCtrl::Encoder::getVel()
{
    return vel;
}

long MotorCtrl::Encoder::getDist()
{
    return position * dist_p_pulse;
}

MotorCtrl::MotorCtrl(byte pin_enableA, byte pin_enableB, byte pin_PWM, byte pin_encoderA, byte pin_encoderB, int dist_p_pulse, void (*f)())
{
    encod = Encoder(pin_encoderA, pin_encoderB, dist_p_pulse, f);
    ponte = PonteH(pin_enableA, pin_enableB, pin_PWM);
}

void MotorCtrl::updateCtrl()
{
    int erro = speedRef - encod.getVel();
    int acao = kp * erro;
    //Serial.println(encod.getVel());
    ponte.setPot(acao);
}

void MotorCtrl::setKP(int kp)
{
    this->kp = kp;
}

void MotorCtrl::encoderPulse()
{
    encod.pulseEncoderA();
}
