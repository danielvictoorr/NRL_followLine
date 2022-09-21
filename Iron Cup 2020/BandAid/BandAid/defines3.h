#define PRETO 0
#define BRANCO 1
#define COR_LINHA BRANCO

int LIMIAR_SENSOR = 600;

#define KP_CURVA 40                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
#define KI_CURVA 0.1//0.75
#define KD_CURVA 23 //5//3

#define KP_RETA 30
#define KI_RETA 0.001
#define KD_RETA 18 //2

float KP = KP_RETA;
float KI = KI_RETA;
float KD = KD_RETA;

#define NUM_CRUZAMENTO 5
//#define NUM_CRUZAMENTO 0
#define NUM_SEGMENTOS 28
//#define NUM_SEGMENTOS 3

#define DX  0.223214  // cm/pulso   -   448 pulsos em 1 metro

#define MultiplicadorReta 1
#define MultiplicadorCurva 1

//int VEL_CURVA[NUM_SEGMENTOS] = {
//                                200,//0
//                                300,//1
//                                300,//2
//  };

int VEL_CURVA[NUM_SEGMENTOS] = {
                                120,//0
                                120,//1
                                120,//2
                                120,//3
                                200,//4
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
                                150,//18
                                120,//19
                                90,//20
                                120,//21
                                90,//22
                                90,//23
                                90,//24
                                90,//25
                                90,//26
                                90,//27
  };

//int SEG_RETA[] = {0,1,3,4,8,9,11,12,14,15,17,19,21,22,24,25,27};
int SEG_RETA[] = {0,1,3,5,8,9,11,12,14,16,17,19,21,22,24,25,27};

float MIN_SENSOR[5] = {1023,1023,1023,1023,1023}, MAX_SENSOR[5] = {0,0,0,0,0};
int PESOS_SENSORES[5] = {-2,-1, 0, 1, 2};


#define CALIBRANDO 2 
#define SEGUINDO 1 
#define PARADO 0
int STATUS = PARADO;
