#include <stdio.h>
#include <Arduino.h>
#include <TimeLib.h>

#define getName(var)  #var

#include "Estacion.h"
#include "sendData.h"
#include "luzindicadora.h"
#include <SFE_BMP180.h>
SFE_BMP180 bmp180;

#define LUCES true    // true para activar juego de luces al encender y enviar datos

#define TEST true     // true para modo test, sin espera de 1 minuto 

#define TIME_THRESHOLD 150


//------------ Variables para controlar pluviometro ------------------------

volatile long int contadorPluv = 0;
long startTime = 0;  //para anti rebote.
long int initialTime = 0;

void pulseDetector();

Estacion estacion = Estacion(0);

void setup() {
  Serial.begin(9600);
  bmp180.begin();

  pinMode(SENSOR_VEL_VIENTO_ENV, INPUT);
  pinMode(SENSOR_DIR_VIENTO_ENV, INPUT);
  pinMode(SENSOR_RADIACION_ENV, INPUT);
  pinMode(SENSOR_TEMPERATURA_ENV, INPUT);
  pinMode(SENSOR_HOJA_ENV, INPUT);

  for(int i=2; i<12; i++){
      pinMode(i, OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(PLUVIOMETRO_PORT), pulseDetector, RISING); // Interrupción por flanco de subida

  LUCES ? loadEffect() : lightsOff();
 
}

void loop() {
  
  time_t t = now();
  
  if ((second(t) == 30 && millis() - startTime > 1000) || TEST) {

    estacion.setVelocidadViento(analogRead(SENSOR_VEL_VIENTO_ENV));       //se leen las entradas analogicas Estación meteorológica.
    estacion.setDireccionViento(analogRead(SENSOR_DIR_VIENTO_ENV));
    estacion.setHumedad(analogRead(SENSOR_HUMEDAD_ENV));
    estacion.setRadiacion(analogRead(SENSOR_RADIACION_ENV));
    estacion.setTemperatura(bmp180);
    estacion.setPresion(bmp180);
    estacion.setHoja(analogRead(SENSOR_HOJA_ENV));

    LUCES ? loadEffect() : lightsOff();  // Efecto de luces

    Serial.println("JSON GENERADO:");
    Serial.println(setPayload(estacion));
    startTime = millis();
    lightsOff(); // Apagamos las luces
    delay(1000);
  }
}

void pulseDetector(){
  // TODO: agregar una activacion de un led para indicar que se ha producido una interrupcion e iniciar un contador de tiempo
    if(millis() - initialTime > TIME_THRESHOLD){
        contadorPluv++;
        estacion.cuentaPulsos(contadorPluv);    
        initialTime = millis();
      }
}
