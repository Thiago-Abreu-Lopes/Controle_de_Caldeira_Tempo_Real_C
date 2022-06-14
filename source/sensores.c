/* Monitor sensor, no arquivo sensor.c */ 
#include <math.h>
#include <pthread.h>
#include <string.h>
//MUTEX DE CAPTURA DO VALOR DOS SENSORES NA CALDEIRA
static pthread_mutex_t exclusao_mutua_PT = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado à temperatura 
static pthread_cond_t alarme = PTHREAD_COND_INITIALIZER;             //Condicional para o alarme de temperatura
static pthread_mutex_t exclusao_mutua_PH = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao nível da água
static pthread_mutex_t exclusao_mutua_PTAMB = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao sensor de temperatura ambiente
static pthread_mutex_t exclusao_mutua_PTAIN = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao sensor de temperatura da água que entra
static pthread_mutex_t exclusao_mutua_PFAOUT = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao sensor do fluxo de água que sai

//MUTEX DE LEITURA DOS VALORES DOS SENSORES NA CALDEIRA
static pthread_mutex_t exclusao_mutua_GT = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado à temperatura
static pthread_mutex_t exclusao_mutua_GH = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao nível da água
static pthread_mutex_t exclusao_mutua_GTAMB = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao sensor de temperatura ambiente
static pthread_mutex_t exclusao_mutua_GTAIN = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao sensor de temperatura da água que entra
static pthread_mutex_t exclusao_mutua_GFAOUT = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao sensor do fluxo de água que sai

static double s_temp = 0, s_nivel = 0, s_tempAmbiente = 0, s_tempAguaEntra = 0, s_fluxoAguaSai = 0; //Variáveis referente a cada sensor
static double limite_atual = HUGE_VAL; //inicializa o limite atual como valor INF

/* Chamado pela thread que le o sensor e disponibiliza aqui o valor lido */

/*#########################################PUTS###########################################*/
//Monitor dedicado ao sensor de temperatura
 void sensor_putT( double temp) {                   
	 pthread_mutex_lock( &exclusao_mutua_PT); 
	 s_temp = temp;
	 if( s_temp >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock( &exclusao_mutua_PT); 
 }
//Monitor dedicado ao sensor de nível
 void sensor_putH(double nivel) { 
	 pthread_mutex_lock( &exclusao_mutua_PH); 
	 s_nivel = nivel;
	 pthread_mutex_unlock( &exclusao_mutua_PH); 
 }
//Monitor dedicado ao sensor de temperatura ambiente
 void sensor_putTAMB(double tempAmb) {
	 pthread_mutex_lock( &exclusao_mutua_PTAMB); 
     s_tempAmbiente = tempAmb;
	 pthread_mutex_unlock( &exclusao_mutua_PTAMB); 
 }
//Monitor dedicado ao sensor de temperatura da água que entra
 void sensor_putTAIN(double tempAguaE) { 
	 pthread_mutex_lock( &exclusao_mutua_PTAIN); 
	 s_tempAguaEntra = tempAguaE;
	 pthread_mutex_unlock( &exclusao_mutua_PTAIN); 
 }
//Monitor dedicado ao sensor do fluxo de água que sai
 void sensor_putFAOUT(double fluxoAguaS) { 
	 pthread_mutex_lock( &exclusao_mutua_PFAOUT); 
     s_fluxoAguaSai = fluxoAguaS;
	 pthread_mutex_unlock( &exclusao_mutua_PFAOUT); 
 }

 /* Chamado por qualquer thread que precisa do valor lido do sensor */ 
/*#########################################GETS###########################################*/
//Monitor dedicado ao sensor de temperatura
 double sensor_getT(char s[5]) {
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GT); 
	 if (strncmp(s,"t",1)==0)
	 aux = s_temp;
	 pthread_mutex_unlock( &exclusao_mutua_GT); 
	 return aux;
 }
//Monitor dedicado ao sensor de nível
 double sensor_getH(char s[5]) { 
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GH); 
	 if(strncmp(s,"h",1)==0)
	 aux = s_nivel;
	 pthread_mutex_unlock( &exclusao_mutua_GH); 
	 return aux;
 }
//Monitor dedicado ao sensor de temperatura ambiente
 double sensor_getTAMB(char s[5]) {
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GTAMB); 
	 if(strncmp(s,"a",1)==0)
     aux = s_tempAmbiente;
	 pthread_mutex_unlock( &exclusao_mutua_GTAMB); 
	 return aux;
 }
//Monitor dedicado ao sensor de temperatura da água que entra
 double sensor_getTAIN(char s[5]) { 
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GTAIN); 
	 if(strncmp(s,"i",1)==0)
     aux = 	s_tempAguaEntra;
	 pthread_mutex_unlock( &exclusao_mutua_GTAIN); 
	 return aux;
 }
//Monitor dedicado ao sensor do fluxo de água que sai
 double sensor_getFAOUT(char s[5]) { 
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GFAOUT); 
	 if (strncmp(s,"o",1)==0)
     aux = s_fluxoAguaSai;
	 pthread_mutex_unlock( &exclusao_mutua_GFAOUT); 
	 return aux;
 }

/*#########################################ALARM###########################################*/
/* Thread fica bloqueada até o valor do sensor chegar em limite */ 
void sensor_alarmeT( double limite) {
	pthread_mutex_lock( &exclusao_mutua); 
	limite_atual = limite; 
	while( s_temp < limite_atual) 
		pthread_cond_wait( &alarme, &exclusao_mutua); 
	limite_atual = HUGE_VAL; 
	pthread_mutex_unlock( &exclusao_mutua); 
}
