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
  Serial1.begin(115200, SERIAL_8N1, 19, 5); //RX, TX
  
  Serial.println(sendATCommand(Serial1, AT_RESET));
  Serial.println(sendATCommand(Serial1, AT_BAUD_115200_CONFIG_SET));
  Serial.println(sendATCommand(Serial1, AT_P2P_CONFIG_SET));
  Serial.println(sendATCommand(Serial1, AT_P2P_CONFIG_GET));
  Serial.println(sendATCommand(Serial1, AT_CONTINUOUS_PRECV_CONFIG_SET));

  weatherStation.init();

  pinMode(WIND_SPEED_SENSOR_PORT, INPUT);
  pinMode(WIND_DIRECTION_SENSOR_PORT, INPUT);
  pinMode(RADIATION_SENSOR_PORT, INPUT);
  pinMode(HUMIDITY_SENSOR_PORT, INPUT);
  pinMode(TEMPERATURE_SENSOR_PORT, INPUT);
  pinMode(LEAF_MOISTURE_SENSOR_PORT, INPUT);
  pinMode(IRRIGATION_CONTROL_PORT, OUTPUT);

  digitalWrite(IRRIGATION_CONTROL_PORT, LOW);

  attachInterrupt(digitalPinToInterrupt(PLUVIOMETRO_PORT), pulseDetector, RISING); // Interrupción por flanco de subida

  Serial.println("Setup finished");
}

void loop() {
  if(Serial1.available()>0) {
    String rxData = readSerial(Serial1);
    rxData.trim();
    rxData = hexToASCII(rxData.substring(rxData.lastIndexOf(':')+1));
    Serial.print("Instruction received: ");
    Serial.println(rxData);
    if (rxData.equals("POLL") || rxData.equals("IRR")) {
      weatherStation.setWindSpeed(analogRead(WIND_SPEED_SENSOR_PORT));       //se leen las entradas analogicas Estación meteorológica.
      weatherStation.setwindDirection(analogRead(WIND_DIRECTION_SENSOR_PORT));
      weatherStation.setHumidity(HUMIDITY_SENSOR_PORT);
      weatherStation.setRadiation(analogRead(RADIATION_SENSOR_PORT));
      weatherStation.setTemperature();
      weatherStation.setPresion();
      weatherStation.setLeafMoisture(analogRead(LEAF_MOISTURE_SENSOR_PORT));
      weatherStation.setPulseCounter(contadorPluv);

      String transmitionPacket = weatherStation.getPayload();

      if (rxData.equals("IRR")) {
        Serial.println(transmitionPacket);
        transmitionPacket = transmitionPacket.substring(0, transmitionPacket.length()-1);
        Serial.println(transmitionPacket);
        transmitionPacket += ",\"dryweight\":" + String(weatherStation.getLysimeterWeight()) + ",";
        Serial.println(transmitionPacket);
        Serial.println("\nOpening irrigation control...");
        digitalWrite(IRRIGATION_CONTROL_PORT, HIGH);
        delay(2000);
        Serial.println("\nClosing irrigation control...");
        digitalWrite(IRRIGATION_CONTROL_PORT, LOW);
        transmitionPacket += "\"wetweight\":" + String(weatherStation.getLysimeterWeight()) + "}";
        Serial.println(transmitionPacket);
      }

      Serial.print("Sending packet:");
      Serial.println(transmitionPacket);
      sendATCommand(Serial1, AT_P2P_CONFIG_TX_SET);
      String response = sendP2PPacket(Serial1, transmitionPacket); 
      Serial.print("Response: ");
      response.replace('\n', ' ');
      Serial.println(response);

      

      sendATCommand(Serial1, AT_CONTINUOUS_PRECV_CONFIG_SET);

      contadorPluv = 0;
      startTime = millis();
      delay(300);
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