#include <stdio.h>
#include <Arduino.h>
#include <TimeLib.h>

#include "WeatherStation.h"
#define AT_RESET "ATZ"
#define AT_BAUD_CONFIG_SET "AT+BAUD=115200"
#define AT_P2P_CONFIG_SET "AT+P2P=915000000:7:0:0:10:14"
#define AT_P2P_CONFIG_GET "AT+P2P=?"
#define AT_P2P_CONFIG_TX_SET "AT+PRECV=0"

#define TEST true     // true para modo test, sin espera de 1 minuto 
#define TIME_THRESHOLD 150

volatile static long int contadorPluv = 0;
long startTime = 0;  //para anti rebote.
long int initialTime = 0;

void pulseDetector();
String sendATCommand(String);
String readSerial2();
String hexToASCII(String);
String asciiToHex(String);
WeatherStation weatherStation;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  sendATCommand(AT_RESET);
  sendATCommand(AT_BAUD_CONFIG_SET);
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

    String sendPacketP2P = "AT+PSEND=" + asciiToHex(weatherStation.getPayload());
    Serial.println(sendPacketP2P);
    String sendPacketResponse = sendATCommand(sendPacketP2P);
    Serial.println(sendPacketResponse);

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

String sendATCommand(String command) {
  String response = "";
  bool configCommand = command.indexOf('?') == -1;
  if(configCommand) {
    delay(1000);
  }
  Serial2.flush();
  Serial2.println();
  Serial2.println(command);
  delay(500);
  Serial2.flush();
  response = readSerial2();
  if( !configCommand ) {
    response = response.substring(0, response.indexOf('\r'));
  }
  response.trim();
  return response;
}

String readSerial2() {
  String readed = "";
  while(Serial2.available()>0) {
    char c = Serial2.read();
    Serial.print(c);
    readed += c;
  }
  return readed;
}

String hexToASCII(String hex) {
  String ascii = "";
  for(int i=0; i<hex.length(); i+=2) {
    String ch = hex.substring(i, i+2);
    char c = (char) (int)strtol(ch.c_str(), NULL, HEX);
    ascii += (char) c;
  }
  return ascii;
}

String asciiToHex(String ascii) {
  String hex = "";
  for(int i=0; i<ascii.length(); i++) {
    char c = ascii.charAt(i);
    int code = c;
    String h = String(code, HEX);
    hex += h;
  }
  return hex;
}