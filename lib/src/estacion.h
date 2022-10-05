#include <Arduino.h>
#ifndef ESTACION_H
#define ESTACION_H
#include <SFE_BMP180.h>

long int setVelocidadViento(long int);
String setDireccionViento(int);

unsigned long int setHumedad(int);

unsigned long int setRadiacion(long int);

long int setTemperatura(double, SFE_BMP180);
long int setPresion(double);

String setHoja(int);

float pluviometro (int);

void cuentaPulsos ();
void resetContadorPluv ();

float fmap(float x, float in_min, float in_max, float out_min, float out_max);

#endif