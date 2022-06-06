//Definição de Bibliotecas
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tela.h"
#include "sensores.h"
#include "socket.h"
#include "referenciaT.h"
#include "bufduplo.h"

#define	NSEC_PER_SEC    (1000000000) 	// Numero de nanosegundos em um segundo
#define N_AMOSTRAS      10000
/*4º requesito[INICIO] - mostrar na tela os valores atualizados dos sensores do sistema*/
void thread_mostra_status (void){
	double t, h, i ,o, a;
	while(1){
		t = sensor_get("t");
		h = sensor_get("h");
		a = sensor_get("a");
		i = sensor_get("i");
		o = sensor_get("o");
		aloca_tela();
		system("tput reset");//limpa a tela(não oculta,limpa msm!!)
		printf("---------------------------------------\n");
        printf("\033[0;32m");
		printf("Temperatura (T)--> %.2lf\n", t);
		printf("Nivel (H)--> %.2lf\n", h);
        printf("\033[0m");
        printf("Temperatura do Ambiente (Ta)--> %.2lf\n", a);
		printf("Temperatura da Agua que Entra (Ti)--> %.2lf\n", i);
        printf("Fluxo da Agua que Sai (No)--> %.2lf\n", o);
		printf("---------------------------------------\n");
		libera_tela();
		sleep(1);
										
	}	
		
}

/*4º requesito [MEIO] - ler os sensores para imprimir pela função acima*/
void thread_le_sensor (void){ //Le Sensores periodicamente a cada 10ms
	
    int cont = 0;
    int intervalo = 100; //determina de quantos em quantos 'thread-le-sensor's' devo salvar o valor do sensor no .txt
	char msg_enviada[1000];
	struct timespec t, t_fim;
	long periodo = 10e6; //10e6ns ou 10ms
	
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);
	while(1){
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		//Envia mensagem via canal de comunicação para receber valores de sensores		
		sensor_put(msg_socket("st-0"), msg_socket("sh-0"),msg_socket("sta0"),msg_socket("sti0"),msg_socket("sno0"));

		//8º requesito [INICIO] - gravação dos valores dos sensores.
        intervalo--;
        if(intervalo == 0 && cont < N_AMOSTRAS){
            FILE * dados_t;
	        dados_t = fopen("sensor_t.txt", "w");
	        if(dados_t ==NULL){
		        printf("Erro na abertura do arquivo do sensor de temperatura\n");
		        exit(1);
	        }
            FILE * dados_h;
	        dados_h = fopen("sensor_h.txt", "w");
	        if(dados_h ==NULL){
		        printf("Erro na abertura do arquivo do sensor de nivel\n");
		        exit(1);
	        }
            fprintf(dados_t, "%2lf\n", sensor_get("t"));
            fprintf(dados_h, "%2lf\n", sensor_get("h"));
            fclose(dados_t);
            fclose(dados_h);
        
            intervalo = 100;
            cont++;        
        }
        //8º requesito [FIM]

		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}		
	}		
}
/*4º requesito [FIM]*/

/*5º requesito [INICIO] - estabelecer o alarme para 30 graus*/
void thread_alarme (void){
	
	while(1){
		sensor_alarmeT(30);
		aloca_tela();
        printf("\033[0;31m");
		printf("ALARME\n");
        printf("\033[0m");
		libera_tela();
		sleep(1);	
	}
		
}
/*5º requesito [FIM]*/

///Controle

/*1º requesito [INICIO] - Controle da temperatura*/
void thread_controle_temperatura (void){
	char msg_enviada[1000];
	long atraso_fim;
	struct timespec t, t_fim;
	long periodo = 50e6; //50ms
	double temp, ref_temp;
	//le a hora atual
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1){
		//esperar até o próximo periodo			
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		temp = sensor_get("t");
		ref_temp = ref_getT();
		double ni, na;
		
		if(temp>ref_temp){
			sprintf(msg_enviada, "ani%lf", 100.0);		
			msg_socket(msg_enviada);

			sprintf(msg_enviada, "anf%lf", 100.0);		
			msg_socket(msg_enviada);
			
			sprintf(msg_enviada, "ana%lf", 0.0);		
			msg_socket(msg_enviada);
		}

		if(temp< ref_temp){
			 if((ref_temp-temp)*20>10.0)
	        		na=10.0;
	        	else
	        		na = (ref_temp-temp)*20;
					
			sprintf( msg_enviada, "ani%lf", 0.0);
	        	msg_socket(msg_enviada);
			
			sprintf( msg_enviada, "anf%lf", 10.0);
	        	msg_socket(msg_enviada);
			
	        	sprintf( msg_enviada, "ana%lf", na);
			msg_socket(msg_enviada);
		}

		// Le a hora atual, coloca em t_fim
		clock_gettime(CLOCK_MONOTONIC ,&t_fim);
		
		// Calcula o tempo de resposta observado em microsegundos
		atraso_fim = 1000000*(t_fim.tv_sec - t.tv_sec)   +   (t_fim.tv_nsec - t.tv_nsec)/1000;
		
		bufduplo_insereLeitura(atraso_fim);
		
		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}	
		
		
	}
}
/*1º requesito [FIM]*/

/*2º requesito [INICIO] - Controle do nível de água*/
void thread_controle_temperatura (void){
	char msg_enviada[1000];
	long atraso_fim;
	struct timespec t, t_fim;
	long periodo = 70e6; //70ms
	double temp, ref_temp;
	//le a hora atual
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1){
		//esperar até o próximo periodo			
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		temp = sensor_get("h");
		ref_temp = ref_getH();
		double ni, na;
		
		if(temp>ref_temp){
			sprintf(msg_enviada, "ani%lf", 100.0);		
			msg_socket(msg_enviada);

			sprintf(msg_enviada, "anf%lf", 100.0);		
			msg_socket(msg_enviada);
			
			sprintf(msg_enviada, "ana%lf", 0.0);		
			msg_socket(msg_enviada);
		}

		if(temp< ref_temp){
			 if((ref_temp-temp)*20>10.0)
	        		na=10.0;
	        	else
	        		na = (ref_temp-temp)*20;
					
			sprintf( msg_enviada, "ani%lf", 0.0);
	        	msg_socket(msg_enviada);
			
			sprintf( msg_enviada, "anf%lf", 10.0);
	        	msg_socket(msg_enviada);
			
	        	sprintf( msg_enviada, "ana%lf", na);
			msg_socket(msg_enviada);
		}

		// Le a hora atual, coloca em t_fim
		clock_gettime(CLOCK_MONOTONIC ,&t_fim);
		
		// Calcula o tempo de resposta observado em microsegundos
		/*atraso_fim = 1000000*(t_fim.tv_sec - t.tv_sec)   +   (t_fim.tv_nsec - t.tv_nsec)/1000;
		
		bufduplo_insereLeitura(atraso_fim);*/ //desnecessário
		
		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}	
		
		
	}
}
/*2º requesito [FIM]*/

/*7º requesito [INICIO] - armazenar tempos de resposta*/
void thread_grava_temp_resp(void){
	FILE * dados_f;
	dados_f = fopen("dados.txt", "w");
	if(dados_f ==NULL){
		printf("Erro na abertura do arquivo \n");
		exit(1);
	}
	
	int amostras = 1;
	while(amostras++<=N_AMOSTRAS/200){
		long * buf = bufduplo_esperaBufferCheio();
		int n2 = tamBuf();
		int tam = 0;
		while(tam<n2)
			fprintf(dados_f, "%4ld\n", buf[tam++]);
		aloca_tela();
		printf("Salvando Arquivo....\n");
		libera_tela();
	}
	fclose(dados_f);			
}
/*7º requesito [FIM]*/

/*6º requesito [INICIO]- atraves do teclado deidimos quais as referencias para o nível e a temperatura*/
double ask_tempRef(){
    double ref_t;
    aloca_tela();
    printf("Digite a referencia para a temperatura --> ");
    scanf("%lf",&ref_t)
    libera_tela();
    return ref_t;
    
}

double ask_nivelRef(){
    double ref_h;
    aloca_tela();
    printf("Digite a referencia para o nivel da agua --> ");
    scanf("%lf",&ref_h)
    libera_tela();
    return ref_h;
    
}
/*6º requesito [FIM]*/

void main( int argc, char *argv[]) {
    ref_putT(ask_tempRef());
    ref_putH(ask_nivelRef());

	cria_socket(argv[1], atoi(argv[2]) );
    
	pthread_t t1, t2, t3, t4, t5;
    
    pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
    pthread_create(&t2, NULL, (void *) thread_le_sensor, NULL);
    pthread_create(&t3, NULL, (void *) thread_alarme, NULL);
    pthread_create(&t4, NULL, (void *) thread_controle_temperatura, NULL);
   	pthread_create(&t5, NULL, (void *) thread_grava_temp_resp, NULL);
    
	pthread_join( t1, NULL);
	pthread_join( t2, NULL);
	pthread_join( t3, NULL);
	pthread_join( t4, NULL);
	pthread_join( t5, NULL);
	    
}
