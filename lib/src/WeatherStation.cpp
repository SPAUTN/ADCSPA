#include <SFE_BMP180.h>
#include <TimeLib.h>

#include "WeatherStation.h"

#define TIME_THRESHOLD 150
#define CALIBRATION 462000.0

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
    vel=(windSpeedSensor * 5 * 48) / 818;
    this -> windSpeed = vel;
    this -> bmp180Sensor.begin();
}

void WeatherStation::setwindDirection(int windDirectionSensor) {
    // Variable que almacena el voltaje (0.0 a 5.0)
    float valorVoltaje = fmap(windDirectionSensor, 0, 1023, 0.0, 5.0);
    this -> windDirection = (int) (valorVoltaje * 100);
}

float WeatherStation::fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void WeatherStation::setHumidity(int humiditySensor) {
    unsigned long int humedad = 0;
    if(humiditySensor<102){
        humedad = 0;
    } else {
        humedad = humiditySensor > 818 ? 100 : ((humiditySensor - 102.f) * 100.f) / 716.f;
    }
    this -> humidity = humedad;
}

void WeatherStation::setRadiation(long int radiationSensor) {
    unsigned long int rad = 0;
    if(radiationSensor < 121){
        rad = 0;
    }
    else {
        rad = ((radiationSensor - 121) * 1400) / 637;
    }
    this -> radiation = rad;
}

void WeatherStation::setTemperature() {
    char status;
    double temperature;
    status = this -> bmp180Sensor.startTemperature();//Inicio de lectura de temperatura
    if (status != 0) {
        delay(status); //Pausa para que finalice la lectura
        status = this -> bmp180Sensor.getTemperature(temperature); //Obtener la temperatura
    }
    this -> temperature = static_cast<long int>(temperature);
}


void WeatherStation::setPresion() {
    double pressure;
    setTemperature();
    double temperature = this -> getTemperature(); //es necesario medir temperature para poder medir la presion
    char status;
    status = bmp180Sensor.startPressure(3); //Inicio lectura de presión
    if (status != 0){        
        delay(status);//Pausa para que finalice la lectura        
        status = bmp180Sensor.getPressure(pressure,temperature); //Obtenemos la presión     
    }      
    this -> pressure = static_cast<long int>(pressure); 
}

void WeatherStation::setLeafMoisture(int leafHumididtySensor) {
    String options[5] = {"Seca", "Poco humeda", "Humeda", "Muy humeda", "Mojada"};
    String resul = options[(int)(leafHumididtySensor/125)-1 > 0 ? (int)(leafHumididtySensor/100)-1 : 0];
    this -> leafMoisture = resul;
}

void WeatherStation::setPulseCounter(long int currentCount) {
    this -> pluviometerCounter += currentCount;
}

void WeatherStation::resetPulseCounter() {
    this -> pluviometerCounter = 0;
}

// -------------------------------------- Getters --------------------------------------

long int WeatherStation::getWindSpeed() {
    return this -> windSpeed;
}

int WeatherStation::getWindDirection() {
    return this -> windDirection;
}

unsigned long int WeatherStation::getHumidity() {
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

String WeatherStation::getLeafMoisture() {
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
  
    jsonPayload += "\"lluvia\":" + String(this ->  getPluviometerCounter());
    jsonPayload += ",\"velocidadViento\":" + String(this ->  getWindSpeed());
    jsonPayload += ",\"humedadHoja\":\"" + String(this ->  getLeafMoisture())+ "\"";
    jsonPayload += ",\"direccionViento\":" + String(this ->  getWindDirection());
    jsonPayload += ",\"humedadRelativa\":" + String(this ->  getHumidity());
    jsonPayload += ",\"radiacionSolar\":" + String(this ->  getRadiation());
    jsonPayload += ",\"temperatura\":" + String(this ->  getTemperature());
    jsonPayload += ",\"presion\":" + String(this ->  getPressure());
    jsonPayload += ",\"peso\":" + String(this ->  getLysimeterWeight(), 4);
    jsonPayload += "}";

    resetPulseCounter();

    return jsonPayload;
}