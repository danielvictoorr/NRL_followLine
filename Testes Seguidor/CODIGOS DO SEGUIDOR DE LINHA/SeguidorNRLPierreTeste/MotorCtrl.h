#include <Arduino.h>

#pragma once

class MotorCtrl
{
  public:
    class PonteH
    {
      private:
        byte pin_enableA;  //ponte h enable A
        byte pin_enableB;  //ponte h enagle B
        byte pin_PWM;      //ponte h PWM
        byte deadZone_PWM; //Zona Morta do motor em PWM
      public:
        /**
         * Construtor que recebe os pinos necessarios.
         */
        PonteH();
        PonteH(byte, byte, byte);
        /**
         * Aciona a ponte com uma potência P até 255. Valores negativos invertem a ponte, 
         * valores abaixo da Zona morta são considerados zero(0). Ponte é desligada com valor zero(0).
         */
        void setPot(int P);
        void setDeadZone(byte deadZone);
    };
    class Encoder
    {
      private:
        byte pin_sinalA; //
        byte pin_sinalB; // se for -1 encoder B não é usado
        bool typeAB;
        volatile long position=0;          // (mm)
        unsigned long lastTime[3]; // (us)
        volatile long pulses=0;            // Pulsos somados entre cada leitura
        int dist_p_pulse=0;       // Distancia (mm) por pulso geralmente = (Diametro da Roda * PI)/Numero de Pulsos por volta
        double vel=0;

      public:
      /**
         * Construtor que recebe os pinos necessarios, a função de interrupção do encoder A.
         */
        Encoder();
        Encoder(byte pin_encoderA, byte pin_encoderB, int dist_p_pulse,  void(*f)());
        /**
         * Construtor que recebe os pinos necessarios.
         */
        Encoder(byte pin_encoderA, int dist_p_pulse,  void(*f)());
        void pulseEncoderA();
        long getVel();  // (mm/s)
        long getDist(); // (mm)
    };

    MotorCtrl::PonteH ponte;
    MotorCtrl::Encoder encod;
    int kp; //constante proporcional do controlador
    int speedRef;

  public:
    MotorCtrl(byte pin_enableA, byte pin_enableB, byte pin_PWM, byte pin_encoderA, byte pin_encoderB, int dist_p_pulse,void(*f)());
    void setKP(int kp);
    void updateCtrl();
    void encoderPulse();
    long getVel(){return encod.getVel();}
    long getDist(){return encod.getDist();}
    void setSpeed(int v){ speedRef = v;} //v em (mm/s)
};