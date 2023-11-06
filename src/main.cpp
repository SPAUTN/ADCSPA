#include <stdio.h>
#include <Arduino.h>
#include <TimeLib.h>
#include <Utils.hpp>
#include <HardwareSerial.h>
#include "WeatherStation.hpp"

#define TEST true     // true para modo test, sin espera de 1 minuto 
#define TIME_THRESHOLD 150
#define AT_CONTINUOUS_PRECV_CONFIG_SET "AT+PRECV=65534" 

volatile static long int contadorPluv = 0;
long startTime = 0;  
long int initialTime = 0;

void pulseDetector();
WeatherStation weatherStation;
ATFunctions atFunctions;
HexFunctions hexFunctions;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 19, 5); //RX, TX
  
  Serial.println(atFunctions.sendATCommand(Serial1, AT_RESET));
  Serial.println(atFunctions.sendATCommand(Serial1, AT_SET_P2P_MODE));
  Serial.println(atFunctions.sendATCommand(Serial1, AT_BAUD_115200_CONFIG_SET));
  Serial.println(atFunctions.sendATCommand(Serial1, AT_P2P_CONFIG_SET));
  Serial.println(atFunctions.sendATCommand(Serial1, AT_P2P_CONFIG_GET));
  Serial.println(atFunctions.sendATCommand(Serial1, AT_CONTINUOUS_PRECV_CONFIG_SET));

  weatherStation.init();

  pinMode(WIND_SPEED_SENSOR_PORT, INPUT);
  pinMode(WIND_DIRECTION_SENSOR_PORT, INPUT);
  pinMode(RADIATION_SENSOR_PORT, INPUT);
  pinMode(HUMIDITY_SENSOR_PORT, INPUT);
  pinMode(TEMPERATURE_SENSOR_PORT, INPUT);
  pinMode(LEAF_MOISTURE_SENSOR_PORT, INPUT);
  pinMode(IRRIGATION_CONTROL_PORT, OUTPUT);

  digitalWrite(IRRIGATION_CONTROL_PORT, LOW);

  attachInterrupt(digitalPinToInterrupt(PLUVIOMETRO_PORT), pulseDetector, RISING); 

  Serial.println("Setup finished");
}

void loop() {
  if(Serial1.available()>0) {
    String rxData = atFunctions.readSerial(Serial1);
    rxData.trim();
    rxData = hexFunctions.hexToASCII(rxData.substring(rxData.lastIndexOf(':')+1));
    Serial.print("Instruction received: ");
    Serial.println(rxData);
  try {
    if (rxData.startsWith(POLL_COMMAND) || rxData.startsWith(IRR_COMMAND)) {
        weatherStation.setWindSpeed(analogRead(WIND_SPEED_SENSOR_PORT));       
        weatherStation.setwindDirection(analogRead(WIND_DIRECTION_SENSOR_PORT));
        weatherStation.setHumidity();
        weatherStation.setRadiation(analogRead(RADIATION_SENSOR_PORT));
        weatherStation.setTemperature();
        weatherStation.setPresion();
        weatherStation.setLeafMoisture(analogRead(LEAF_MOISTURE_SENSOR_PORT));
        weatherStation.setPulseCounter(contadorPluv);

        String transmitionPacket = weatherStation.getPayload();

        if (rxData.startsWith(IRR_COMMAND)) {
          // Divide the string into parts using the semicolon as a delimiter
          // String parts[3]; 
          // int semicolonIndex = -1;
          // for (int i = 0; i < 3; i++) {
          //   semicolonIndex = rxData.indexOf(';');
          //   if (semicolonIndex != -1) {
          //     parts[i] = rxData.substring(0, semicolonIndex);
          //     rxData = rxData.substring(semicolonIndex + 1);
          //   } else {
          //     parts[i] = rxData;
          //     break;
          //   }
          // }
          String commmandData = rxData.substring(rxData.indexOf(';')+1);
          float wetweight = commmandData.substring(0, commmandData.indexOf(';')+1).toFloat();
          float rain = commmandData.substring(commmandData.indexOf(';')+1).toFloat();

          //float wetweight = parts[1].toFloat();       
          //float rain = parts[2].toFloat(); 
          float ETc = weatherStation.irrigateAndGetETc(wetweight, rain);    //controla el riego con wetweight y la lluvia consultada

          transmitionPacket = transmitionPacket.substring(0, transmitionPacket.length()-1);
          transmitionPacket += ",\"etc\":" + String(ETc, 2) + ",";
          Serial.println(transmitionPacket);
          transmitionPacket += "\"wetweight\":" + String(weatherStation.getLysimeterWeight()) + "}";
          Serial.println(transmitionPacket);
        }

        Serial.print("Sending packet:");
        Serial.println(transmitionPacket);
        atFunctions.sendATCommand(Serial1, AT_P2P_CONFIG_TX_SET);
        String response = atFunctions.sendP2PPacket(Serial1, transmitionPacket); 
        Serial.print("Response: ");
        response.replace('\n', ' ');
        Serial.println(response);

        atFunctions.sendATCommand(Serial1, AT_CONTINUOUS_PRECV_CONFIG_SET);

        contadorPluv = 0;
        startTime = millis();
        delay(300);
      }  
    } catch (std::runtime_error& e) {
      Serial.println(e.what());
      atFunctions.sendATCommand(Serial1, AT_P2P_CONFIG_TX_SET);
      atFunctions.sendP2PPacket(Serial1, e.what());
      atFunctions.sendATCommand(Serial1, AT_CONTINUOUS_PRECV_CONFIG_SET);
    }
  }
}

void pulseDetector() {
    if(millis() - initialTime > TIME_THRESHOLD){
        contadorPluv++;
        initialTime = millis();
    }
}