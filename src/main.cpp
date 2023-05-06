#include <stdio.h>
#include <Arduino.h>
#include <TimeLib.h>
#include <Utils.h>
#include <HardwareSerial.h>
#include "WeatherStation.h"

#define TEST true     // true para modo test, sin espera de 1 minuto 
#define TIME_THRESHOLD 150
#define AT_CONTINUOUS_PRECV_CONFIG_SET "AT+PRECV=65534" 

volatile static long int contadorPluv = 0;
long startTime = 0;  //para anti rebote.
long int initialTime = 0;

void pulseDetector();
WeatherStation weatherStation;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 19, 5);
  String response1 = sendATCommand(Serial1, AT_RESET);
  String response2 = sendATCommand(Serial1, AT_BAUD_115200_CONFIG_SET);
  String response3 = sendATCommand(Serial1, AT_P2P_CONFIG_SET);
  String response4 = sendATCommand(Serial1, AT_P2P_CONFIG_GET);
  String response5 = sendATCommand(Serial1, AT_CONTINUOUS_PRECV_CONFIG_SET);
  //String response5 = sendATCommand(Serial1, AT_P2P_CONFIG_TX_SET);

  weatherStation.init();

  pinMode(WIND_SPEED_SENSOR_PORT, INPUT);
  pinMode(WIND_DIRECTION_SENSOR_PORT, INPUT);
  pinMode(RADIATION_SENSOR_PORT, INPUT);
  pinMode(HUMIDITY_SENSOR_PORT, INPUT);
  pinMode(TEMPERATURE_SENSOR_PORT, INPUT);
  pinMode(LEAF_MOISTURE_SENSOR_PORT, INPUT);

  attachInterrupt(digitalPinToInterrupt(PLUVIOMETRO_PORT), pulseDetector, RISING); // Interrupción por flanco de subida

  Serial.print("Response1: ");
  Serial.println(response1);
  Serial.print("Response2: ");
  Serial.println(response2);
  Serial.print("Response3: ");
  Serial.println(response3);
  Serial.print("Response4: ");
  Serial.println(response4);
  Serial.print("Response5: ");
  Serial.println(response5);

  Serial.println("Setup finished");
}

void loop() {
  if(Serial1.available()>0) {
    String rxData = readSerial(Serial1);
    rxData.trim();
    rxData = hexToASCII(rxData.substring(rxData.lastIndexOf(':')+1));
    Serial.print("Instruction received: ");
    Serial.println(rxData);
    if (rxData.equals("POLL")) {
      weatherStation.setWindSpeed(analogRead(WIND_SPEED_SENSOR_PORT));       //se leen las entradas analogicas Estación meteorológica.
      weatherStation.setwindDirection(analogRead(WIND_DIRECTION_SENSOR_PORT));
      weatherStation.setHumidity(analogRead(HUMIDITY_SENSOR_PORT));
      weatherStation.setRadiation(analogRead(RADIATION_SENSOR_PORT));
      weatherStation.setTemperature();
      weatherStation.setPresion();
      weatherStation.setLeafMoisture(analogRead(LEAF_MOISTURE_SENSOR_PORT));
      weatherStation.setPulseCounter(contadorPluv);

      Serial.print("Sending packet:");
      Serial.println(weatherStation.getPayload());
      sendATCommand(Serial1, AT_P2P_CONFIG_TX_SET);
      String response = sendP2PPacket(Serial1, weatherStation.getPayload()); 
      Serial.print("Response: ");
      response.replace('\n', ' ');
      Serial.println(response);
      sendATCommand(Serial1, AT_CONTINUOUS_PRECV_CONFIG_SET);

      contadorPluv = 0;
      startTime = millis();
      delay(3000);
    }
  }
}

void pulseDetector(){
  // TODO: agregar una activacion de un led para indicar que se ha producido una interrupcion e iniciar un contador de tiempo
    if(millis() - initialTime > TIME_THRESHOLD){
        contadorPluv++;
        initialTime = millis();
    }
}