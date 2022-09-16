#include <Arduino.h>
#include "estacion.h"

#define TIME_THRESHOLD 150

extern volatile long contadorPluv;

long initTime = 0;

/**
 * @brief Calcula la velocidad del viento. Convertimos a Km/h teniendo en cuenta que
 * como maximo llegan 4V (818)(240km/h)
 * 
 * @param sensorVel del sensor
 * @return long int valor de velocidad del viento
 */
long int setVelocidadViento(long int sensorVel) {
  long int vel=0;
   vel=(sensorVel * 5 * 48) / 818;
   return vel;
}

/**
 * @brief Setea la dirección del viento
 * 
 * @param sensorDir valor del sensor
 * @return String dirViento que indica el nombre del punto cardinal donde apunta la dirección del viento
 */
String setDireccionViento(int sensorDir) {
  String dirViento = "";
  if(sensorDir>39 && sensorDir<103){
    dirViento = "NORTE";
  }
  if(sensorDir>715 && sensorDir<779){
    dirViento = "NORTE";
  }
  if(sensorDir>306 && sensorDir<431){
    dirViento = "SUR";
  }
  if(sensorDir>122 && sensorDir<247){
    dirViento = "ESTE";
  }
  if(sensorDir>490 && sensorDir<615){
    dirViento = "OESTE";
  }
  return dirViento;
}

/**
 * @brief Setea la humedad del ambiente entre 0% y 100%
 * 
 * @param sensorHum valor del sensor de SENSOR_HUMEDAD
 * @return uint humedad porcentaje de SENSOR_HUMEDAD
 */
unsigned long int setHumedad(int sensorHum) {
  unsigned long int humedad = 0;
  if(sensorHum<102){
    humedad = 0;
  }
  if(sensorHum>102){
    humedad = ((sensorHum - 102) * 5 * 20) / 716;
  }
  return humedad;
}

/**
 * @brief Setea la radiación solar entre 0 y 1400 W/m2
 * 
 * @param sensorRad valor leido del sensor de radiación solar
 * @return uint rad valor de radiacion solar
 */
unsigned long int setRadiacion(long int sensorRad) {
  long int rad = 0;
  if(sensorRad < 41){
    rad = 0;
  }
  if(sensorRad > 41){
     rad = ((sensorRad - 41) * 5 * 280) / 716;
  }
  return rad;
}

/**
 * @brief Setea la temperatura ambiente entre -25°C y 50°C
 * 
 * @param sensorTemp valor leido del sensor de temperatura
 * @return long int temp valor real de temperatura
 */
long int setTemperatura(int sensorTemp) {
  long int temp = 0;
  if(sensorTemp < 41){
    temp = -25;
  }
  if(sensorTemp > 41){
    temp = (((sensorTemp - 41) * 5 * 15) / 696) - 25;
  }
  return temp;
}

/**
 * @brief Setea si la hoja del campo esta mojada o seca
 * 
 * @param sensHoja valor leido del sensor
 * @return String result indicando si la hoja está mojada o seca
 */
String setHoja(int sensHoja) {
  String options[5] = {"Seca", "Poco humeda", "Humeda", "Muy humeda", "Mojada"};
  String resul = options[(int)(sensHoja/125)-1 > 0 ? (int)(sensHoja/100)-1 : 0];
  return resul;
}

// Pluviometro
/**
 * @brief Contador de pulsos digitales para pluviometro
 * 
 * @param contador cantidad de pulsos digitales
 * @return int 
 */
float pluviometro (int contador) {
  // Un valor habitual de Paso es 0,25 mm lo cual significa que por cada 1 mm de
  // precipitación se producirán 4 vuelcos de cangilón
    return contador*0.25;
}


/**
 * @brief funcion ISR que aumenta el numero de pulsos al producirse
 * una interrupcion por flanco de subida en el pin.
 * Además debe indicar cuando se ha producido el primer conteo para que
 * se pueda calcular el tiempo de lluvia
 */
void cuentaPulsos () {
  // TODO: agregar una activacion de un led para indicar que se ha producido una interrupcion e iniciar un contador de tiempo
  if(millis() - initTime > TIME_THRESHOLD){
    // Serial.println("Interrupcion");
    contadorPluv++;    
    initTime = millis();
  }
}

/**
 * @brief funcion para volver a cero el contador de pulsos del pluviometro
 * debe determinarse si hacerlo cada cierto tiempo o mediante una señal
 * externa de reset.
 * 
 */
void resetContadorPluv () {
  contadorPluv = 0;
}