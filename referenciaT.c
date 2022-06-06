/* Monitor sensor, no arquivo sensor.c */ 
#include <math.h>
#include <pthread.h>

static pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER; 
static double ref_entradaTemp = 0, ref_entradaNivel = 0; 
static double limite_atual = HUGE_VAL;


/* Chamado pela thread que le o sensor de temperatura e disponibiliza aqui o valor lido */
 void ref_putT( double ref) {
	 pthread_mutex_lock( &exclusao_mutua); 
	 ref_entradaTemp = ref; 
	 pthread_mutex_unlock( &exclusao_mutua); 
 }
 
 /* Chamado por qualquer thread que precisa do valor lido do sensor de temperatura */ 
 double ref_getT( void) {
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua); 
	 aux = ref_entradaTemp; 
	 pthread_mutex_unlock( &exclusao_mutua); 
	 return aux;
 }

/* Chamado pela thread que le o sensor de nivel e disponibiliza aqui o valor lido */
 void ref_putH( double ref) {
	 pthread_mutex_lock( &exclusao_mutua); 
	 ref_entradaNivel = ref; 
	 pthread_mutex_unlock( &exclusao_mutua); 
 }
 
 /* Chamado por qualquer thread que precisa do valor lido do sensor de nivel */ 
 double ref_getH( void) {
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua); 
	 aux = ref_entradaNivel; 
	 pthread_mutex_unlock( &exclusao_mutua); 
	 return aux;
 }
