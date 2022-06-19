/* Monitor atuadores, no arquivo atuador.c */ 
#include <math.h>
#include <pthread.h>
#include <string.h>

static double a_q = 10000.0, a_ni = 0.0, a_na = 0.0, a_nf = 0.0; //Variáveis referente a cada atuador
static pthread_mutex_t exclusao_mutua_PQ = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao atuador de fluxo de calor Q
static pthread_mutex_t exclusao_mutua_PNI = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao atuador do fluxo de agua que entran NI
static pthread_mutex_t exclusao_mutua_PNA = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao atuador do fluxo de agua a 80° que entra NA
static pthread_mutex_t exclusao_mutua_PNF = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao sensor do fluxo de água vai para o esgoto NF

static pthread_mutex_t exclusao_mutua_GQ = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao atuador de fluxo de calor Qa
static pthread_mutex_t exclusao_mutua_GNI = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao atuador do fluxo de agua que entran NI
static pthread_mutex_t exclusao_mutua_GNA = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao atuador do fluxo de agua a 80° que entra NAa
static pthread_mutex_t exclusao_mutua_GNF = PTHREAD_MUTEX_INITIALIZER; //Mutex dedicado ao sensor do fluxo de água vai para o esgoto NF


//Monitor dedicado ao atuador do fluxo de calor do elemento Q
 void atuador_putQ(double q){ 
	 pthread_mutex_lock( &exclusao_mutua_PQ); 
     a_q = q;
	 pthread_mutex_unlock( &exclusao_mutua_PQ); 
 }
//Monitor dedicado ao atuador do fluxo de agua que entra NI
 void atuador_putNI(double ni){ 
	 pthread_mutex_lock( &exclusao_mutua_PNI); 
     a_ni = ni;
	 pthread_mutex_unlock( &exclusao_mutua_PNI); 
 }
//Monitor dedicado ao atuador do fluxo de agua acima de 80° que entra NA
 void atuador_putNA(double na){ 
	 pthread_mutex_lock( &exclusao_mutua_PNA); 
     a_na = na;
	 pthread_mutex_unlock( &exclusao_mutua_PNA); 
 }
//Monitor dedicado ao atuador do fluxo de agua que vai para o esgoto NF
 void atuador_putNF(double nf){ 
	 pthread_mutex_lock( &exclusao_mutua_PNF); 
     a_nf = nf;
	 pthread_mutex_unlock( &exclusao_mutua_PNF); 
 }

//Monitor dedicado ao atuador do fluxo de calor do elemento Q
 double atuador_getQ(){ 
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GQ); 
     aux = a_q;
	 pthread_mutex_unlock( &exclusao_mutua_GQ); 
	 return aux;
 }
//Monitor dedicado ao atuador do fluxo de agua que entra NI
 double atuador_getNI(){ 
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GNI); 
     aux = a_ni;
	 pthread_mutex_unlock( &exclusao_mutua_GNI); 
	 return aux;
 }
//Monitor dedicado ao atuador do fluxo de agua a 80° que entra NA
 double atuador_getNA(){ 
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GNA); 
     aux = a_na;
	 pthread_mutex_unlock( &exclusao_mutua_GNA); 
	 return aux;
 }
//Monitor dedicado ao atuador do fluxo de agua que vai para o esgoto NF
 double atuador_getNF(){ 
	 double aux; 
	 pthread_mutex_lock( &exclusao_mutua_GNF); 
     aux = a_nf;
	 pthread_mutex_unlock( &exclusao_mutua_GNF); 
	 return aux;
 }
