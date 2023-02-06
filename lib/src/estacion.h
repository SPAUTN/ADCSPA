#include <Arduino.h>
#ifndef ESTACION_H
#define ESTACION_H
#include <SFE_BMP180.h>
#include "HX711.h"

long int setVelocidadViento(long int);
int setDireccionViento(int);

unsigned long int setHumedad(int);

unsigned long int setRadiacion(long int);


long int setTemperatura(SFE_BMP180);
long int setPresion(SFE_BMP180);

String setHoja(int);

float pluviometro (int);

void cuentaPulsos ();
void resetContadorPluv ();

float fmap(float x, float in_min, float in_max, float out_min, float out_max);

float setLisimetro(HX711);

#endif