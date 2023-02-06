#ifndef UNIT_TEST

#include <stdio.h>
#include <Arduino.h>
#include <TimeLib.h>
#include "HX711.h"
//#include <LowPower.h>

#define getName(var)  #var
#define DEBUG_HX711
// Parámetro para calibrar el peso y el sensor (ya calibrado)
#define CALIBRACION 462000.0

#include "estacion.h"
#include "sendData.h"
#include "luzindicadora.h"
#include <SFE_BMP180.h>
SFE_BMP180 bmp180;
HX711 lisimetro;


//-------------- Definición de entradas lisimetro -----------------------
byte pinData = 16; //pin RX2
byte pinClk = 4;  //pin D4

//-------------- Definición de entradas analogicas -----------------------
#define SENSOR_VEL_VIENTO 30
#define SENSOR_DIR_VIENTO 29
#define SENSOR_RADIACION 26
#define SENSOR_HUMEDAD 25
#define SENSOR_TEMPERATURA 24
#define SENSOR_HOJA 23
#define LUCES true   //true para activar juego de luces al encender y enviar datos

//const int CELDAS[4] = {A6, A7, A8, A9};

//------------- Definición entradas digitales -----------------------------
#define SENSOR_PLUVIOMETRO 18    

//------------ Variables para almacenar los datos -------------------------
unsigned long int velViento = 0;
int dirViento = 0;
unsigned long int radiacion = 0;
long int temperatura = 0;
long int presion = 0;
unsigned long int humedad = 0;
String hojaMojada = "";
float peso = 0;

int celdas[4] = {0, 0, 0, 0};

//------------ Variables para controlar pluviometro ------------------------

volatile long int contadorPluv = 0;
int pulsosXhora = 0;
int pulsosXdia = 0;
float precipDIA = 0;
float precipHORA = 0;
long startTime = 0;  //para anti rebote.
time_t t;

void setup() {
  Serial.begin(9600);
  bmp180.begin();
  lisimetro.begin(pinData, pinClk);
  lisimetro.set_scale(CALIBRACION);
  lisimetro.tare();

  pinMode(SENSOR_VEL_VIENTO, INPUT);
  pinMode(SENSOR_DIR_VIENTO, INPUT);
  pinMode(SENSOR_RADIACION, INPUT);
  pinMode(SENSOR_TEMPERATURA, INPUT);
  pinMode(SENSOR_HOJA, INPUT);

 /* for (int i = 0; i < 4; i++) {
    pinMode(CELDAS[i], INPUT);
  }

  for(int i=2; i<12; i++){
      pinMode(i, OUTPUT);
  }
*/

  attachInterrupt(digitalPinToInterrupt(SENSOR_PLUVIOMETRO), cuentaPulsos, RISING); // Interrupción por flanco de subida

  LUCES ? loadEffect() : lightsOff();
 
}

void loop() {
  
  t = now();                                  // Declaramos la variable time_t 

  if (second(t)==58 && millis() - startTime > 1000) {

    /*for (int i = 0; i < 4; i++) {            // lectura de las entradas analogicas Lisimetro 
      celdas[i] = analogRead(CELDAS[i]);
    }*/

    velViento = setVelocidadViento(analogRead(SENSOR_VEL_VIENTO));       //se leen las entradas analogicas Estación meteorológica.
    dirViento = setDireccionViento(analogRead(SENSOR_DIR_VIENTO));
    humedad = setHumedad(analogRead(SENSOR_HUMEDAD));
    radiacion = setRadiacion(analogRead(SENSOR_RADIACION));
    temperatura = setTemperatura(bmp180);
    presion = setPresion(bmp180);
    hojaMojada = setHoja(analogRead(SENSOR_HOJA));
    peso = setLisimetro(lisimetro);
    LUCES ? loadEffect() : lightsOff();  // Efecto de luces

    Serial.println("JSON GENERADO:");
    Serial.println(setPayload());
    startTime = millis();
    lightsOff(); // Apagamos las luces
    delay(500);
   // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_ON); // Apagamos el microcontrolador durante 8 segundos. 
  }
}

#endif