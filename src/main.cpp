#include <stdio.h>
#include <Arduino.h>
#include <TimeLib.h>
#include <SPI.h>

#include "WeatherStation.h"
//----------#include "luzindicadora.h"

#define LUCES false    // true para activar juego de luces al encender y enviar datos
#define TEST true     // true para modo test, sin espera de 1 minuto 
#define TIME_THRESHOLD 150

volatile static long int contadorPluv = 0;
long startTime = 0;  //para anti rebote.
long int initialTime = 0;

SPIClass Hspi(HSPI);

void pulseDetector();
WeatherStation weatherStation;

void setup() {
  Serial.begin(9600);
  Serial.println("MISO: " + String(MISO));
  Serial.println("MOSI: " + String(MOSI));
  Serial.println("SCK: " + String(SCK));
  Serial.println("SS: " + String(SS));

  //Hspi.begin(HSPI_SCLK
  pinMode(MISO, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);

  weatherStation.init();

  pinMode(WIND_SPEED_SENSOR_PORT, INPUT);
  pinMode(WIND_DIRECTION_SENSOR_PORT, INPUT);
  pinMode(RADIATION_SENSOR_PORT, INPUT);
  pinMode(HUMIDITY_SENSOR_PORT, INPUT);
  pinMode(TEMPERATURE_SENSOR_PORT, INPUT);
  pinMode(LEAF_MOISTURE_SENSOR_PORT, INPUT);

  // TODO: determinar los 10 pines a usar para las luces indicadoras en esp32
  /*for(int i=2; i<12; i++){
      pinMode(i, OUTPUT);
  }*/

  attachInterrupt(digitalPinToInterrupt(PLUVIOMETRO_PORT), pulseDetector, RISING); // Interrupción por flanco de subida

  //LUCES ? loadEffect() : lightsOff();
 
}

void loop() {
  
  time_t t = now();  
  if ((second(t) == 30 && millis() - startTime > 1000) || TEST) {

    weatherStation.setWindSpeed(analogRead(WIND_SPEED_SENSOR_PORT));       //se leen las entradas analogicas Estación meteorológica.
    weatherStation.setwindDirection(analogRead(WIND_DIRECTION_SENSOR_PORT));
    weatherStation.setHumidity(analogRead(HUMIDITY_SENSOR_PORT));
    weatherStation.setRadiation(analogRead(RADIATION_SENSOR_PORT));
    weatherStation.setTemperature();
    weatherStation.setPresion();
    weatherStation.setLeafMoisture(analogRead(LEAF_MOISTURE_SENSOR_PORT));
    weatherStation.setPulseCounter(contadorPluv);

    //LUCES ? loadEffect() : lightsOff();  // Efecto de luces

    Serial.println(weatherStation.getPayload());
    SPI.transfer32((const char *) weatherStation.getPayload().c_str()); // Enviamos los datos por SPI

    contadorPluv = 0;
    startTime = millis();
    //lightsOff(); // Apagamos las luces
    delay(1000);
  }
}

void pulseDetector(){
  // TODO: agregar una activacion de un led para indicar que se ha producido una interrupcion e iniciar un contador de tiempo
    if(millis() - initialTime > TIME_THRESHOLD){
        contadorPluv++;
        initialTime = millis();
    }
}
