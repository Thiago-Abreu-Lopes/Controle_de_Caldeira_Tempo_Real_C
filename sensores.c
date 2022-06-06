/* Monitor sensor, no arquivo sensor.c */ 
#include <math.h>
#include <pthread.h>
#include <string.h>

static pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER; 
static pthread_cond_t alarme = PTHREAD_COND_INITIALIZER; 
static double s_temp = 0, s_nivel = 0, s_tempAmbiente = 0, s_tempAguaEntra = 0, s_fluxoAguaSai = 0; /*inclusão dos novos sensores*/
static double limite_atual = HUGE_VAL;

/* Chamado pela thread que le o sensor e disponibiliza aqui o valor lido */
 void sensor_put( double temp, double nivel, double tempAmb, double tempAguaE, double fluxoAguaS) { /*inclusão dos novos sensores*/
	 pthread_mutex_lock( &exclusao_mutua); 
	 s_temp = temp;
	 s_nivel = nivel;
     s_tempAmbiente = tempAmb;
	 s_tempAguaEntra = tempAguaE;
     s_fluxoAguaSai = fluxoAguaS;
	 if( s_temp >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock( &exclusao_mutua); 
 }
 
 /* Chamado por qualquer thread que precisa do valor lido do sensor */ 
 double sensor_get(char s[5]) { /*inclusão dos novos sensores*/
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua); 
	 if (strncmp(s,"t",1)==0)
	 aux = s_temp;
	 else if(strncmp(s,"h",1)==0)
	 aux = s_nivel;
     else if(strncmp(s,"a",1)==0)
     aux = s_tempAmbiente;
     else if(strncmp(s,"i",1)==0)
     aux = 	s_tempAguaEntra;
     else if (strncmp(s,"o",1)==0)
     aux = s_fluxoAguaSai;
	 pthread_mutex_unlock( &exclusao_mutua); 
	 return aux;
 }

/* Thread fica bloqueada até o valor do sensor chegar em limite */ 
void sensor_alarmeT( double limite) {
	pthread_mutex_lock( &exclusao_mutua); 
	limite_atual = limite; 
	while( s_temp < limite_atual) 
		pthread_cond_wait( &alarme, &exclusao_mutua); 
	limite_atual = HUGE_VAL; 
	pthread_mutex_unlock( &exclusao_mutua); 
}
