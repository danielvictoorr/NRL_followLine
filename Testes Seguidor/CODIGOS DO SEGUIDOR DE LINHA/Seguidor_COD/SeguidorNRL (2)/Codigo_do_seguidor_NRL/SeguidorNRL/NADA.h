struct Motor {
  int pin_1;
  int pin_2;
  int pin_PWM;

  long posicao;

  int dist_p_pulso = (40 * 3.14)/15; //mm por pulso  - 80mm de diametro da roda

  int pulsos = 0;
  unsigned int pulso_time = 0; //Em ms
  unsigned int pulso_dt = 0; //Em ms

  int vel_ref = 0;

  long getVel() {
    if (pulso_dt <= 0  || (millis() - pulso_time) > 900)
      return 0;
    return ((1000L * dist_p_pulso / pulso_dt) );
  }

  void updateVel() {
    //digitalWrite(PINMOTOR,vel_ref);
    long vel_atual = getVel();
    long erro = vel_ref - vel_atual;
    //if(vel_ref > 0) erro = vel_ref - vel_atual;
    //else erro = vel_atual - vel_ref;
    //Serial.print("Erro: ");
    //Serial.println(erro);
    long acao = 1*erro;

    //Serial.println(acao);

    //Zona morta
    if (abs(acao) >= 30) {
      //acao += 30;
      if (acao > 0) { // Inverter Ponte H
        digitalWrite(pin_1, HIGH);
        digitalWrite(pin_2, LOW);
      }
      else {
        //acao = -acao;
        digitalWrite(pin_1, LOW);
        digitalWrite(pin_2, HIGH);
      }
      //Saturação
      acao = acao > 255 ? 255 : acao;
      analogWrite(pin_PWM, acao);
    }
    else {
      digitalWrite(pin_1, LOW);
      digitalWrite(pin_2, LOW);
      digitalWrite(pin_PWM, 0);
    }

    if(vel_ref==0){
      digitalWrite(pin_1, LOW);
      digitalWrite(pin_2, LOW);
      digitalWrite(pin_PWM, 0);
    }
  }

  void setupPins(int p1, int p2, int pwm) {
    pin_1 = p1;
    pin_2 = p2;
    pin_PWM = pwm;

    pinMode(pin_1, OUTPUT);
    pinMode(pin_2, OUTPUT);
    pinMode(pin_PWM, OUTPUT);

    digitalWrite(pin_1, LOW);
    digitalWrite(pin_2, LOW);
    digitalWrite(pin_PWM, LOW);
  }
};

struct Motor MA;
struct Motor MB;

void pulsoContA() {
  MA.pulso_dt =  millis() - MA.pulso_time;
  MA.pulso_time = millis();
  MA.pulsos++;
  MA.posicao++;
}

void pulsoContB() {
  MB.pulso_dt =  millis() - MB.pulso_time;
  MB.pulso_time = millis();
  MB.pulsos++;
  MB.posicao++;
}