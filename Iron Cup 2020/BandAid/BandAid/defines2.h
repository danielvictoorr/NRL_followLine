#define PRETO 0
#define BRANCO 1
#define COR_LINHA BRANCO

#define LIMIAR_SENSOR 800

#define KP_CURVA 8                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
#define KI_CURVA 0//0.1//0.75
#define KD_CURVA 1//5//3

#define KP_RETA 1.6
#define KI_RETA 0.00
#define KD_RETA 0.8//2

float KP = KP_RETA;
float KI = KI_RETA;
float KD = KD_RETA;

#define NUM_CRUZAMENTO 5 //6
#define NUM_SEGMENTOS 28 //29

#define DX  0.3930131004  //dx = 0.3930131004 cm/pulso

#define MultiplicadorReta 0.1
#define MultiplicadorCurva 0.1
int VEL_CURVA[NUM_SEGMENTOS] = {
                                100,//0
                                200,//1
                                110,//2
                                180,//3
                                140,//4
                                100,//5
                                120,//6
                                100,//7
                                150,//8
                                150,//9
                                120,//10
                                150,//11
                                150,//12
                                120,//13
                                150,//14
                                150,//15
                                120,//16
                                150,//17
                                130,//18
                                150,//19
                                90,//20
                                200,//21
                                150,//22
                                120,//23
                                150,//24
                                150,//25
                                90,//26
                                150,//27
  };

int SEG_RETA[] = {0,1,3,5,8,9,11,12,14,15,17,19,21,22,24,25,27};
//int SEG_RETA[] = {0,1,2,3,4,9,11,12,14,16,17,19,21,22,24,25,27};

float MIN_SENSOR[5] = {1023,1023,1023,1023,1023}, MAX_SENSOR[5] = {0,0,0,0,0};
int PESOS_SENSORES[5] = {-2,-1, 0, 1, 2};

#define CALIBRANDO 2 
#define SEGUINDO 1 
#define PARADO 0
int STATUS = PARADO;
