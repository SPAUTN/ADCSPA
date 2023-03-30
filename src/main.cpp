#include <stdio.h>
#include <Arduino.h>
#include <TimeLib.h>
#include <Utils.h>
#include "WeatherStation.h"

#define TEST true     // true para modo test, sin espera de 1 minuto 
#define TIME_THRESHOLD 150

volatile static long int contadorPluv = 0;
long startTime = 0;  //para anti rebote.
long int initialTime = 0;

void pulseDetector();
WeatherStation weatherStation;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  sendATCommand(AT_RESET);
  sendATCommand(AT_BAUD_115200_CONFIG_SET);
  sendATCommand(AT_P2P_CONFIG_SET);
  sendATCommand(AT_P2P_CONFIG_GET);
  sendATCommand(AT_P2P_CONFIG_TX_SET);

  weatherStation.init();

  pinMode(WIND_SPEED_SENSOR_PORT, INPUT);
  pinMode(WIND_DIRECTION_SENSOR_PORT, INPUT);
  pinMode(RADIATION_SENSOR_PORT, INPUT);
  pinMode(HUMIDITY_SENSOR_PORT, INPUT);
  pinMode(TEMPERATURE_SENSOR_PORT, INPUT);
  pinMode(LEAF_MOISTURE_SENSOR_PORT, INPUT);

  attachInterrupt(digitalPinToInterrupt(PLUVIOMETRO_PORT), pulseDetector, RISING); // Interrupción por flanco de subida

  Serial.println("Setup finished");
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

    Serial.println("Sending packet...");
    String response = sendP2PPacket(weatherStation.getPayload()); 
    Serial.print("Response: ");
    Serial.println(response);

    contadorPluv = 0;
    startTime = millis();
    delay(3000);
  }
}

void pulseDetector(){
  // TODO: agregar una activacion de un led para indicar que se ha producido una interrupcion e iniciar un contador de tiempo
    if(millis() - initialTime > TIME_THRESHOLD){
        contadorPluv++;
        initialTime = millis();
    }
}