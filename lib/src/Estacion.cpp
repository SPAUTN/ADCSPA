#include <SFE_BMP180.h>
#include <TimeLib.h>

#include "Estacion.h"

#define TIME_THRESHOLD 150
#define CALIBRACION 462000.0

Estacion::Estacion(long contadorPluv){
    this -> initTime = 0;
    this -> contadorPluv = contadorPluv;
}

Estacion::Estacion(){
    this -> initTime = 0;
    this -> contadorPluv = 0;
}
void Estacion::init(){
    this -> bmp180.begin();
    this -> lisimetro.begin(16, 4);
    this -> lisimetro.set_scale(CALIBRACION);
    this -> lisimetro.tare();
}

void Estacion::setVelocidadViento(long int sensorVel) {
    long int vel=0;
    vel=(sensorVel * 5 * 48) / 818;
    this -> velViento = vel;
}

void Estacion::setDireccionViento(int sensorDir) {
    // Variable que almacena el voltaje (0.0 a 5.0)
    float valorVoltaje = fmap(sensorDir, 0, 1023, 0.0, 5.0);
    this -> dirViento = (int) (valorVoltaje * 100);
}

float Estacion::fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Estacion::setHumedad(int sensorHum) {
    unsigned long int humedad = 0;
    if(sensorHum<102){
        humedad = 0;
    } else {
        humedad = sensorHum > 818 ? 100 : ((sensorHum - 102.f) * 100.f) / 716.f;
    }
    this -> humedad = humedad;
}

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

void Estacion::setTemperatura() {
    char status;
    double temperatura;
    status = this -> bmp180.startTemperature();//Inicio de lectura de temperatura
    if (status != 0) {
        delay(status); //Pausa para que finalice la lectura
        status = this -> bmp180.getTemperature(temperatura); //Obtener la temperatura
    }
    this -> temperatura = static_cast<long int>(temperatura);
}


void Estacion::setPresion(){
    double presion;
    setTemperatura();
    double temperatura = this -> getTemperatura(); //es necesario medir temperatura para poder medir la presion
    char status;
    status = this -> bmp180.startPressure(3); //Inicio lectura de presión
    if (status != 0){        
        delay(status);//Pausa para que finalice la lectura        
        status = this -> bmp180.getPressure(presion,temperatura); //Obtenemos la presión     
    }      
    this -> presion = static_cast<long int>(presion); 
}

void Estacion::setHoja(int sensHoja) {
    String options[5] = {"Seca", "Poco humeda", "Humeda", "Muy humeda", "Mojada"};
    String resul = options[(int)(sensHoja/125)-1 > 0 ? (int)(sensHoja/100)-1 : 0];
    this -> humHoja = resul;
}

void Estacion::cuentaPulsos (long int pulsos) {
    this -> contadorPluv = pulsos;
}

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

float Estacion::getPesoLisimetro(){
    return this -> lisimetro.get_units(4);
}

SFE_BMP180 Estacion::getTempModulo(){
    return this -> bmp180;
}