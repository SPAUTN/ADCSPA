#ifndef UNIT_TEST

#include <stdio.h>
#include <Arduino.h>
#include <TimeLib.h>
#include <LowPower.h>

#define getName(var)  #var

#include "Estacion.h"
#include "sendData.h"
#include "luzindicadora.h"
#include <SFE_BMP180.h>
SFE_BMP180 bmp180;

//-------------- Definición de entradas analogicas -----------------------
#define SENSOR_VEL_VIENTO A0
#define SENSOR_DIR_VIENTO A1
#define SENSOR_RADIACION A2
#define SENSOR_HUMEDAD A3
#define SENSOR_TEMPERATURA A4
#define SENSOR_HOJA A5
#define LUCES true   //true para activar juego de luces al encender y enviar datos

#define TEST false

#define TIME_THRESHOLD 150

const int CELDAS[4] = {A6, A7, A8, A9};

//------------- Definición entradas digitales -----------------------------
#define SENSOR_PLUVIOMETRO 18     
int celdas[4] = {0, 0, 0, 0};

//------------ Variables para controlar pluviometro ------------------------

volatile long int contadorPluv = 0;
long startTime = 0;  //para anti rebote.
long int initialTime = 0;

void pulseDetector();

Estacion estacion = Estacion(0);

void setup() {
  Serial.begin(9600);
  bmp180.begin();

  pinMode(SENSOR_VEL_VIENTO, INPUT);
  pinMode(SENSOR_DIR_VIENTO, INPUT);
  pinMode(SENSOR_RADIACION, INPUT);
  pinMode(SENSOR_TEMPERATURA, INPUT);
  pinMode(SENSOR_HOJA, INPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(CELDAS[i], INPUT);
  }

  for(int i=2; i<12; i++){
      pinMode(i, OUTPUT);
  }


  attachInterrupt(digitalPinToInterrupt(SENSOR_PLUVIOMETRO), pulseDetector, RISING); // Interrupción por flanco de subida

  LUCES ? loadEffect() : lightsOff();
 
}

void loop() {

  time_t t = now();
  
  if (second(t) == 30 && millis() - startTime > 1000 || TEST) {

    for (int i = 0; i < 4; i++) {            // lectura de las entradas analogicas Lisimetro 
      celdas[i] = analogRead(CELDAS[i]);
    }

    estacion.setVelocidadViento(analogRead(SENSOR_VEL_VIENTO));       //se leen las entradas analogicas Estación meteorológica.
    estacion.setDireccionViento(analogRead(SENSOR_DIR_VIENTO));
    estacion.setHumedad(analogRead(SENSOR_HUMEDAD));
    estacion.setRadiacion(analogRead(SENSOR_RADIACION));
    estacion.setTemperatura(bmp180);
    estacion.setPresion(bmp180);
    estacion.setHoja(analogRead(SENSOR_HOJA));

    LUCES ? loadEffect() : lightsOff();  // Efecto de luces

    Serial.println("JSON GENERADO:");
    Serial.println(setPayload(estacion));
    startTime = millis();
    lightsOff(); // Apagamos las luces
    delay(1000);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_ON); // Apagamos el microcontrolador durante 8 segundos. 
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

#endif