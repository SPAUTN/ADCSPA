#include <stdio.h>
#include <Arduino.h>
#include <SFE_BMP180.h>
#include <TimeLib.h>

#include "DHT.h"
#include "WeatherStation.h"

#define TIME_THRESHOLD 150
#define CALIBRATION 468.6

WeatherStation::WeatherStation(long initialCounter) {
    this -> initTime = 0;
    this -> pluviometerCounter = initialCounter;
}

WeatherStation::WeatherStation() {
    this -> initTime = 0;
    this -> pluviometerCounter = 0;
}
void WeatherStation::init() {
    this -> lysimeter.begin(16, 4);
    this -> lysimeter.set_scale(CALIBRATION);
    this -> lysimeter.tare();
    this -> bmp180Sensor.begin();
}

void WeatherStation::setWindSpeed(long int windSpeedSensor) {
    long int vel=0;
    if ( windSpeedSensor >= 6) {
        vel = ((windSpeedSensor - 6) * 240) / 2972;
    }
    this -> windSpeed = vel;
}

void WeatherStation::setwindDirection(int windDirectionSensor) {
    
    float valorVoltaje = fmap(windDirectionSensor, 0, 4095, 0.0, 3.3);
    this -> windDirection = (int) (valorVoltaje * 100);
}

float WeatherStation::fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void WeatherStation::setHumidity(int humiditySensorPort) {
    delay(2000); //Para respetar la frecuencia del sensor
    DHT sensorTH (humiditySensorPort, DHT22);
    this -> humidity = isnan(sensorTH.readHumidity()) ? 0 : sensorTH.readHumidity();
}

void WeatherStation::setRadiation(long int radiationSensor) {
    unsigned long int rad = 0;
    if (radiationSensor >= 10) {
        rad = ((radiationSensor - 10) * 1400) / 2596;
    }
    this -> radiation = rad > 1400 ? 1400 : rad;
}

void WeatherStation::setTemperature() {
    char status;
    double temperature;
    status = this -> bmp180Sensor.startTemperature();
    if (status != 0) {
        delay(status); 
        status = this -> bmp180Sensor.getTemperature(temperature); 
    }
    this -> temperature = static_cast<long int>(temperature);
}


void WeatherStation::setPresion() {
    double pressure;
    setTemperature();
    double temperature = this -> getTemperature(); 
    char status;
    status = bmp180Sensor.startPressure(3); 
    if (status != 0){        
        delay(status);
        status = bmp180Sensor.getPressure(pressure,temperature); 
    }      
    this -> pressure = static_cast<long int>(pressure); 
}


void WeatherStation::setLeafMoisture(int leafHumididtySensor) {
    this -> leafMoisture = round((leafHumididtySensor*100)/2668);
}

void WeatherStation::setPulseCounter(long int currentCount) {
    this -> pluviometerCounter += currentCount;
}

void WeatherStation::resetPulseCounter() {
    this -> pluviometerCounter = 0;
}

void WeatherStation::plantIrrigation(int ETc, int lluvia) {
    float PesoSecoHoy = getLysimeterWeight();
    float pesoActual = 0;      //declaracion de variable
    float densidadAgua = 1.0; // Densidad del agua en g/cm³
    
    int aguaNecesaria = ETc - lluvia;
    if (aguaNecesaria < 0) {
        aguaNecesaria = 0; // No se necesita riego si la lluvia es suficiente.
    }
    float volumen = (aguaNecesaria/10) * 1225;        // Convertir mm a cm³ (1225 cm2 supuesta area lisimetro)
    float RiegoRequerido = volumen * densidadAgua;    // peso en gramos
    Serial.println("\nLa cantidad de agua a regar en gramos es: ");
    Serial.println(RiegoRequerido);                   //verificación del peso que debe agregarse

    Serial.println("\nOpening irrigation control...");
    digitalWrite(IRRIGATION_CONTROL_PORT, HIGH);

    // Realizar el riego hasta que se alcance el peso objetivo
    do {
        pesoActual = getLysimeterWeight();
        delay(100); // Esperar 0.1 segundo entre lecturas de peso
    } while (pesoActual < PesoSecoHoy + RiegoRequerido);

    Serial.println("\nClosing irrigation control...");
    digitalWrite(IRRIGATION_CONTROL_PORT, LOW);
}


long int WeatherStation::getWindSpeed() {
    return this -> windSpeed;
}

int WeatherStation::getWindDirection() {
    return this -> windDirection;
}

float WeatherStation::getHumidity() {
    return this -> humidity;
}

unsigned long int WeatherStation::getRadiation() {
    return this -> radiation;
}

long int WeatherStation::getTemperature() {
    return this -> temperature;
}

long int WeatherStation::getPressure() {
    return this -> pressure;
}

int WeatherStation::getLeafMoisture() {
    return this -> leafMoisture;
}

float WeatherStation::getPluviometerCounter() {
    return this -> pluviometerCounter*0.25;
}

float WeatherStation::getLysimeterWeight() {
    return this -> lysimeter.get_units(4);
}


String WeatherStation::getPayload() {
    String jsonPayload = "{";
    jsonPayload += "\"pluviometer\":" + String(this ->  getPluviometerCounter());
    jsonPayload += ",\"windspeed\":" + String(this ->  getWindSpeed());
    jsonPayload += ",\"winddirection\":" + String(this ->  getWindDirection());
    jsonPayload += ",\"leafmoisture\":" + String(this ->  getLeafMoisture());
    jsonPayload += ",\"humidity\":" + String(this ->  getHumidity());
    jsonPayload += ",\"radiation\":" + String(this ->  getRadiation());
    jsonPayload += ",\"temperature\":" + String(this ->  getTemperature());
    jsonPayload += ",\"pressure\":" + String(this ->  getPressure());
    jsonPayload += ",\"weight\":" + String(this ->  getLysimeterWeight(), 4);
    jsonPayload += "}";

    this -> resetPulseCounter();

    return jsonPayload;
}