#include <QTRSensors.h>
/*Não fomos nós que fizemos*/
/////////////////////////////////////////////////////////////////////////////////////
//*************  Programa realizado por MARCO A. CABALLERO MORENO***************   //
// Solo para fines educativos   
// robot velocista mini FK BOT  V 2.0                                            //
// micro motores pololu MP 10:1, sensores qtr 8rc, driver drv8833, arduino nano    //
//                                     05/12/2014                  
// ACTUALIZADO 29/3/2015            
/////////////////////////////////////////////////////////////////////////////////////
 
#define NUM_SENSORS   8    //numero de sensores usados
#define TIMEOUT       2000  // tempo de espera para dar resultado en uS
#define EMITTER_PIN   6     //pin led on
///////////////pinos arduino a utilizar/////////////////////
#define led1          13 
#define led2          4  
#define mot_i         7
#define mot_d         8
#define sensores      8
#define boton_1       2  //pin para botão
#define pin_pwm_i     9
#define pin_pwm_d     10
 
QTRSensorsRC qtrrc((unsigned char[]) {19, 18, 17, 16,15,14,11,12}
,NUM_SENSORS, TIMEOUT, EMITTER_PIN);
 
//variaVEIS para armazenar valores de sensores e posicão
unsigned int sensorValues[NUM_SENSORS];
unsigned int position=0;
 
/// variaveis para o pid
int  derivativo=0, proporcional=0, integral=0; //parametros
int  salida_pwm=0, proporcional_pasado=0;
 
///////////////AQUI MUDAMOS OS PARÂMETROS DO NOSSO ROBÔ!!!!!!!!!!!!!!!
int velocidad=120;              //variaveis para a velocidade, no maximo e 255
float Kp=0.18, Kd=4, Ki=0.001;  //constantes
//Variável para escolher o tipo de linha
int linea=0;                    //  0 para linha pretas, 1 para linha brancas
 
void setup()
{
 delay(800);
 pinMode(mot_i, OUTPUT);//pin de direção motor esquerdo
 pinMode(mot_d, OUTPUT);//pin de direção motor direito
 pinMode(led1, OUTPUT); //led1
 pinMode(led2, OUTPUT); //led2
 pinMode(boton_1, INPUT); //boton 1 como pull up
         
 for (int i = 0; i <40; i++)  //calibração durante 2.5 segundos,
 {                                 //para calibrar e necesario colocar os sensores sobre a superficie negra 
  digitalWrite(led1, HIGH);  //a branca
  delay(20);
  qtrrc.calibrate();    //função para calibrar sensores   
  digitalWrite(led1, LOW); 
  delay(20);
 }
digitalWrite(led1, LOW); //apagar sensores para indicar fim da calibração 
delay(400); 
digitalWrite(led2,HIGH); //acender led 2 para indicar
while(true)
{
    int x=digitalRead(boton_1); //leemos e guardamos o valor
                                      // do boton na variavel x
    delay(100);
    if(x==0) //se presiona boton 
    {
        digitalWrite(led2,LOW); //indicamos que se precionou o boton
        digitalWrite(led1,HIGH); //acendemos led 1
        delay(100);
        break; //nós saltamos para o loop principal
    }
}
}       
 
 
void loop()
{
 
 //pid(0, 120, 0.18, 0.001, 4 );
  pid(linea,velocidad,Kp,Ki,Kd); //função para algoritmo pid 
                                 //primeiro parametro: 0 para linha negras, tipo 1 para linhas brancas
                                 //segundo parametro: velocidade pwm de 0 a 255
                                 //terceiro parametro: constante para ação proporcional
                                 //quarto parametro: constante para ação integral
                                 //quinto parametro: constante para ação derivativa
  //frenos_contorno(0,700);
  frenos_contorno(linea,700); //função para frenagem no tipo de curvas 
                              //primeiro parâmetro: 0 para linhas pretas, tipo 1 para linhas brancas
                              //segundo parâmetro: borda comparação vai de 0 a 1000, para este
}
 
////////FUNÇÕES PARA O CONTROLE DO ROBÔ////
 void pid(int linea, int velocidad, float Kp, float Ki, float Kd)
{
  position = qtrrc.readLine(sensorValues, QTR_EMITTERS_ON, linea); //0 para linha Afrodescendente, 1 para linha elite branca haha
  proporcional = (position) - 3500; // set point e 3500, ai obtemos o erro
  integral=integral + proporcional_pasado; //obtendo integral
  derivativo = (proporcional - proporcional_pasado); //obtendo o derivativo
  if (integral>1000) integral=1000; //limitamos a integral para não causar problemas
  if (integral<-1000) integral=-1000;
  salida_pwm =( proporcional * Kp ) + ( derivativo * Kd )+(integral*Ki);
   
  if (  salida_pwm > velocidad )  salida_pwm = velocidad; //limitamos a salida de pwm
  if ( salida_pwm < -velocidad )  salida_pwm = -velocidad;
   
  if (salida_pwm < 0)
 {
  motores(velocidad+salida_pwm, velocidad);
 }
 if (salida_pwm >0)
 {
  motores(velocidad, velocidad-salida_pwm);
 }
 
 proporcional_pasado = proporcional;  
}
 
//função para controle de motores
void motores(int motor_izq, int motor_der)
{
  if ( motor_izq >= 0 )  //motor esquerdo
 {
  digitalWrite(mot_i,HIGH); // con high avança
  analogWrite(pin_pwm_i,255-motor_izq); //se controla de manera
                                        //inversa para maior controle
 }
 else
 {
  digitalWrite(mot_i,LOW); //con low retrocede
  motor_izq = motor_izq*(-1); //mudança de sinal
  analogWrite(pin_pwm_i,motor_izq); 
 }
 
  if ( motor_der >= 0 ) //motor direito;
 {
  digitalWrite(mot_d,HIGH);
  analogWrite(pin_pwm_d,255-motor_der);
 }
 else
 {
  digitalWrite(mot_d,LOW);
  motor_der= motor_der*(-1);
  analogWrite(pin_pwm_d,motor_der);
 }
 
   
}
 
void frenos_contorno(int tipo,int flanco_comparacion)
{
   
if(tipo==0)
{
  if(position<=50) //se saiu do lado direito da linha
 {
  motores(-80,90); //devido à inércia, do motor 
                                  //tendera seguir girando
                                  //por isso hle damos para atras , para que freie
                                 // o mas rapido possivel 
  while(true)  
  {
   qtrrc.read(sensorValues); //leitura bruto do sensor   
   if( sensorValues[0]>flanco_comparacion || sensorValues[1]>flanco_comparacion ) 
   //assegurar que esta na linha
   {
    break;
   } 
  }
 }
 
 if (position>=6550) //Se saiu pela parte esquerda da linha
 { 
  motores(90,-80);
  while(true)
  {
   qtrrc.read(sensorValues);
   if(sensorValues[7]>flanco_comparacion || sensorValues[6]>flanco_comparacion )
   {
    break;
   }  
  }
 }
}
 
if(tipo==1) //para linha branca com fundo negro
{
 if(position<=50) //se saiu por la parte dereitcha da linha
 {
  motores(-80,90); //devido a inercia do motor 
                   //tendera a seguir girando
                   //por isso hle damos para atras  
                   //para que freie o mas rapido possivel 
  while(true)  
  {
   qtrrc.read(sensorValues); //letura bruta do sensor
   if(sensorValues[0]<flanco_comparacion || sensorValues[1]<flanco_comparacion )   //assegurar que esta na linha
   {
    break;
   }
  }
 }
 
 if(position>=6550) //Se saiu pela parte esquierda da linha
 { 
  motores(90,-80);
  while(true)
  {
   qtrrc.read(sensorValues);
   if(sensorValues[7]<flanco_comparacion || sensorValues[6]<flanco_comparacion)
   {
    break;
   }  
  }
 }
}
}
