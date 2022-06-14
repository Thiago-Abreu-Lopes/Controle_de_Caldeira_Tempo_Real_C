/* Monitor sensor, no arquivo sensor.c */ 
#include <math.h>
#include <pthread.h>

static pthread_mutex_t exclusao_mutua_T = PTHREAD_MUTEX_INITIALIZER; 
static pthread_mutex_t exclusao_mutua_H = PTHREAD_MUTEX_INITIALIZER; 
static double ref_entradaTemp = 0, ref_entradaNivel = 0; 
static double limite_atual = HUGE_VAL;


/* Chamado pela thread que le o sensor de temperatura e disponibiliza aqui o valor lido */
 void ref_putT( double ref) {
	 pthread_mutex_lock( &exclusao_mutua_T); 
	 ref_entradaTemp = ref; 
	 pthread_mutex_unlock( &exclusao_mutua_T); 
 }
 
 /* Chamado por qualquer thread que precisa do valor lido do sensor de temperatura */ 
 double ref_getT( void) {
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_T); 
	 aux = ref_entradaTemp; 
	 pthread_mutex_unlock( &exclusao_mutua_T); 
	 return aux;
 }

/* Chamado pela thread que le o sensor de nivel e disponibiliza aqui o valor lido */
 void ref_putH( double ref) {
	 pthread_mutex_lock( &exclusao_mutua_H); 
	 ref_entradaNivel = ref; 
	 pthread_mutex_unlock( &exclusao_mutua_H); 
 }
 
 /* Chamado por qualquer thread que precisa do valor lido do sensor de nivel */ 
 double ref_getH( void) {
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_H); 
	 aux = ref_entradaNivel; 
	 pthread_mutex_unlock( &exclusao_mutua_H); 
	 return aux;
 }
