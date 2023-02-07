#include <Arduino.h>
#include <SFE_BMP180.h>
#include <TimeLib.h>

#include "Estacion.h"

#define TIME_THRESHOLD 150

// Constructor
Estacion::Estacion(long contadorPluv){
    this -> initTime = 0;
    this -> contadorPluv = contadorPluv;
}

Estacion::Estacion(){
    this -> initTime = 0;
    this -> contadorPluv = 0;
}

// -------------------------------------- Setters --------------------------------------

/**
 * @brief Calcula la velocidad del viento. Convertimos a Km/h teniendo en cuenta que
 * como maximo llegan 4V (818)(240km/h)
 * 
 * @param sensorVel del sensor
 * @return long int valor de velocidad del viento
 */
void Estacion::setVelocidadViento(long int sensorVel) {
    long int vel=0;
    vel=(sensorVel * 5 * 48) / 818;
    this -> velViento = vel;
}

/**
 * @brief Setea la dirección del viento
 * 
 * @param sensorDir valor del sensor
 * @return int valor de la dirección del viento en grados respecto al norte
 */
void Estacion::setDireccionViento(int sensorDir) {
    // Variable que almacena el voltaje (0.0 a 5.0)
    float valorVoltaje = fmap(sensorDir, 0, 1023, 0.0, 5.0);
    this -> dirViento = (int) (valorVoltaje * 100);
}

// cambio de escala entre floats
float Estacion::fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief Setea la humedad del ambiente entre 0% y 100%
 * 
 * @param sensorHum valor del sensor de SENSOR_HUMEDAD
 * @return uint humedad porcentaje de SENSOR_HUMEDAD
 */
void Estacion::setHumedad(int sensorHum) {
    unsigned long int humedad = 0;
    if(sensorHum<102){
        humedad = 0;
    } else {
        humedad = sensorHum > 818 ? 100 : ((sensorHum - 102.f) * 100.f) / 716.f;
    }
    this -> humedad = humedad;
}

/**
 * @brief Setea la radiación solar entre 0 y 1400 W/m2
 * 
 * @param sensorRad valor leido del sensor de radiación solar
 * @return uint rad valor de radiacion solar
 */
void Estacion::setRadiacion(long int sensorRad) {
    unsigned long int rad = 0;
    if(sensorRad < 121){
        rad = 0;
    }
    else {
        rad = ((sensorRad - 121) * 1400) / 637;
    }
    this -> radiacion = rad;
}

/**
 * @brief Lectura de la temperatura del sensor bmp180
 * 
 * @param status retorno de las funciones del sensor bmp180 (0 o 1)
 * @return long int temperatura valor real de temperatura
 */
void Estacion::setTemperatura(SFE_BMP180 bmp180i) {
    char status;
    double temperatura;
    status = bmp180i.startTemperature();//Inicio de lectura de temperatura
    if (status != 0) {
        delay(status); //Pausa para que finalice la lectura
        status = bmp180i.getTemperature(temperatura); //Obtener la temperatura
    }
    this -> temperatura = static_cast<long int>(temperatura);
}

/**
 * @brief Lectura de la presion del sensor bmp180
 * 
 * @param status retorno de las funciones del sensor bmp180 (0 o 1)
 * @return long int presion valor real de temperatura
 */
void Estacion::setPresion(SFE_BMP180 bmp180i){
    double presion;
    setTemperatura(bmp180i);
    double temperatura = this -> getTemperatura(); //es necesario medir temperatura para poder medir la presion
    char status;
    status = bmp180i.startPressure(3); //Inicio lectura de presión
    if (status != 0){        
        delay(status);//Pausa para que finalice la lectura        
        status = bmp180i.getPressure(presion,temperatura); //Obtenemos la presión     
    }      
    this -> presion = static_cast<long int>(presion); 
}

/**
 * @brief Setea si la hoja del campo esta mojada o seca
 * 
 * @param sensHoja valor leido del sensor
 * @return String result indicando si la hoja está mojada o seca
 */
void Estacion::setHoja(int sensHoja) {
    String options[5] = {"Seca", "Poco humeda", "Humeda", "Muy humeda", "Mojada"};
    String resul = options[(int)(sensHoja/125)-1 > 0 ? (int)(sensHoja/100)-1 : 0];
    this -> humHoja = resul;
}

/**
 * @brief funcion ISR que aumenta el numero de pulsos al producirse
 * una interrupcion por flanco de subida en el pin.
 * Además debe indicar cuando se ha producido el primer conteo para que
 * se pueda calcular el tiempo de lluvia
 */
void Estacion::cuentaPulsos (long int pulsos) {
    this -> contadorPluv = pulsos;
}

/**
 * @brief funcion para volver a cero el contador de pulsos del pluviometro
 * debe determinarse si hacerlo cada cierto tiempo o mediante una señal
 * externa de reset.
 * 
 */
void Estacion::resetContadorPluv () {
    this -> contadorPluv = 0;
}

// -------------------------------------- Getters --------------------------------------

long int Estacion::getVelViento() {
    return this -> velViento;
}

int Estacion::getDirViento (){
    return this -> dirViento;
}

unsigned long int Estacion::getHumedad() {
    return this -> humedad;
}

unsigned long int Estacion::getRadiacion() {
    return this -> radiacion;
}

long int Estacion::getTemperatura() {
    return this -> temperatura;
}

long int Estacion::getPresion() {
    return this -> presion;
}

String Estacion::getHumHoja() {
    return this -> humHoja;
}

long Estacion::getContadorPluv() {
    return this -> contadorPluv*0.25;
}