#include <Arduino.h>
#include "estacion.h"
#include <SFE_BMP180.h>
SFE_BMP180 bmp180i;

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
  float valorVoltaje;            // variable que almacena el voltaje (0.0 a 5.0)
  valorVoltaje = fmap(sensorDir, 0, 1023, 0.0, 5.0);   // cambiar escala a 0.0 - 5.0
  if(valorVoltaje>=0.17 && valorVoltaje<0.40){
    dirViento = "NORTE";
  }
  if(valorVoltaje>=0.40 && valorVoltaje<0.74){
    dirViento = "NORESTE";
  }
  if(valorVoltaje>=0.74 && valorVoltaje<1.17){
    dirViento = "ESTE";
  }
  if(valorVoltaje>=1.17 && valorVoltaje<1.59){
    dirViento = "SURESTE";
  }
  if(valorVoltaje>=1.59 && valorVoltaje<2.){
    dirViento = "SUR";
  }
  if(valorVoltaje>=2.0 && valorVoltaje<2.34){
    dirViento = "SUROESTE";
  }
  if(valorVoltaje>=2.34 && valorVoltaje<2.82){
    dirViento = "OESTE";
  }
  if(valorVoltaje>=2.82 && valorVoltaje<3.15){
    dirViento = "NOROESTE";
  }
  if(valorVoltaje>=3.15 && valorVoltaje<3.60){
    dirViento = "NORTE";
  }
  return dirViento;
}
// cambio de escala entre floats
float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
  } else {
    humedad = sensorHum > 818 ? 100 : ((sensorHum - 102.f) * 100.f) / 716.f;
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
     rad = ((sensorRad - 41) * 1400) / 716;
  }
  return rad;
}

/**
 * @brief Lectura de la temperatura del sensor bmp180
 * 
 * @param status retorno de las funciones del sensor bmp180 (0 o 1)
 * @return long int temperatura valor real de temperatura
 */
long int setTemperatura() {
  double temperatura;
  char status;
  status = bmp180i.startTemperature();//Inicio de lectura de temperatura
  if (status != 0) {
    delay(status); //Pausa para que finalice la lectura
    status = bmp180i.getTemperature(temperatura); //Obtener la temperatura
  }
  return static_cast<long int>(temperatura);
}

/**
 * @brief Lectura de la presion del sensor bmp180
 * 
 * @param status retorno de las funciones del sensor bmp180 (0 o 1)
 * @return long int presion valor real de temperatura
 */
long int setPresion(double presion){
  double temperatura; //es necesario medir temperatura para poder medir la presion
  char status;
  status = bmp180i.startTemperature();//Inicio de lectura de temperatura
  if (status != 0){   
    delay(status); //Pausa para que finalice la lectura
    status = bmp180i.getTemperature(temperatura); //Obtener la temperatura
    if (status != 0){
      status = bmp180i.startPressure(3); //Inicio lectura de presión
      if (status != 0){        
        delay(status);//Pausa para que finalice la lectura        
        status = bmp180i.getPressure(presion,temperatura); //Obtenemos la presión     
      }      
    }   
  }
  return presion; 
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