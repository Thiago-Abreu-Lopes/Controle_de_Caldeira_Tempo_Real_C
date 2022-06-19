#ifndef ATUADORES_H
#define ATUADORES_H

void atuador_putQ(double q);
void atuador_putNI(double ni);
void atuador_putNA(double na);
void atuador_putNF(double nf);

double atuador_getQ(void);
double atuador_getNI(void);
double atuador_getNA(void);
double atuador_getNF(void);

void sensor_alarmeT( double limite);

#endif

