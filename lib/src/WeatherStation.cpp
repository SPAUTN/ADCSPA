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
    sensorTH.begin ();
    this -> humidity = sensorTH.readHumidity();
}

void WeatherStation::setRadiation(long int radiationSensor) {
    unsigned long int rad = 0;
    if(radiationSensor >= 10){
        rad = ((radiationSensor - 10) * 1400) / 2596;
    }
    this -> radiation = rad;
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
    this -> leafMoisture = round((leafHumididtySensor*100)/4095);
}

void WeatherStation::setPulseCounter(long int currentCount) {
    this -> pluviometerCounter += currentCount;
}

void WeatherStation::resetPulseCounter() {
    this -> pluviometerCounter = 0;
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
    jsonPayload += "\"rain_milimeters\":" + String(this ->  getPluviometerCounter());
    jsonPayload += ",\"wind_speed\":" + String(this ->  getWindSpeed());
    jsonPayload += ",\"wind_direction\":" + String(this ->  getWindDirection());
    jsonPayload += ",\"leaf_moisture\":" + String(this ->  getLeafMoisture());
    jsonPayload += ",\"relative_humidity\":" + String(this ->  getHumidity());
    jsonPayload += ",\"solar_radiation\":" + String(this ->  getRadiation());
    jsonPayload += ",\"temperature\":" + String(this ->  getTemperature());
    jsonPayload += ",\"pressure\":" + String(this ->  getPressure());
    jsonPayload += ",\"weight\":" + String(this ->  getLysimeterWeight(), 4);
    jsonPayload += "}";

    this -> resetPulseCounter();

    return jsonPayload;
}