#ifndef SENSORES_H
#define SENSORES_H

void sensor_putT( double temp);
void sensor_putH(double nivel);
void sensor_putTAMB(double tempAmb);
void sensor_putTAIN(double tempAguaE);
void sensor_putFAOUT(double fluxoAguaS);

double sensor_getT(char s[5]);
double sensor_getH(char s[5]);
double sensor_getTAMB(char s[5]);
double sensor_getTAIN(char s[5]);
double sensor_getFAOUT(char s[5]);

void sensor_alarmeT( double limite);

#endif



