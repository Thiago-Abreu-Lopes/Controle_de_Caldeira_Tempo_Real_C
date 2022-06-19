#ifndef SENSORES_H
#define SENSORES_H

void sensor_putT( double temp);
void sensor_putH(double nivel);
void sensor_putTAMB(double tempAmb);
void sensor_putTAIN(double tempAguaE);
void sensor_putFAOUT(double fluxoAguaS);

void atuador_putQ(double q);
void atuador_putNI(double ni);
void atuador_putNA(double na);
void atuador_putNF(double nf);

double sensor_getT(void);
double sensor_getH(void);
double sensor_getTAMB(void);
double sensor_getTAIN(void);
double sensor_getFAOUT(void);

double atuador_getQ(void);
double atuador_getNI(void);
double atuador_getNA(void);
double atuador_getNF(void);

void sensor_alarmeT( double limite);

#endif



