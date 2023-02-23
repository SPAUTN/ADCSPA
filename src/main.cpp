#ifndef UNIT_TEST
#include <Arduino.h>
#include <SFE_BMP180.h>
#include <TimeLib.h>
#include "WeatherStation.h"
#include <stdio.h>
#include <driver/uart.h>

//----------#include "luzindicadora.h"

#define LUCES false    // true para activar juego de luces al encender y enviar datos
#define TEST true     // true para modo test, sin espera de 1 minuto 
#define TIME_THRESHOLD 150

#define UART_NUM UART_NUM_0
#define BUF_SIZE 1024
#define TASK_MEMORY 1024

volatile static long int contadorPluv = 0;
long startTime = 0;  //para anti rebote.
long int initialTime = 0;

void pulseDetector();
WeatherStation weatherStation;

void setup() {
  Serial.begin(9600);

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
  
  // Configuración de UART
  uart_config_t uart_config = {
      .baud_rate = 9600,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_APB
  };
  uart_param_config(UART_NUM, &uart_config);
  uart_set_pin(UART_NUM, 3, 1, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_NUM, BUF_SIZE, BUF_SIZE, 0, NULL, 0);

  ESP_LOGI(tag, "init uart completed!");
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
    uart_write_bytes(UART_NUM, (const char *) weatherStation.getPayload().c_str(), strlen(weatherStation.getPayload().c_str()));
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
#endif