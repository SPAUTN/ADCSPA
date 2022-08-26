#include <Arduino.h>

#include <Time.h>
#include <TimeLib.h>

//-------------- Definición de entradas analogicas -----------------------
#define SENSOR_VEL_VIENTO A0
#define SENSOR_DIR_VIENTO A1
#define SENSOR_RADIACION A2
#define SENSOR_HUMEDAD A3
#define SENSOR_TEMPERATURA A4
#define SENSOR_HOJA A5
#define CELDA_1 A6
#define CELDA_2 A7
#define CELDA_3 A8
#define CELDA_4 A9

//------------- Definición entradas digitales -----------------------------
#define SENSOR_PLUVIOMETRO 18     

//------------ Constantes -------------------------
#define TIME_THRESHOLD 150

//------------ Variables para almacenar los datos -------------------------
unsigned long int velViento = 0;
String dirViento = "";
unsigned long int radiacion = 0;
long int temperatura = 0;
unsigned long int humedad = 0;
String hojaMojada = "";
int Celda1 = 0;
int Celda2 = 0;
int Celda3 = 0;
int Celda4 = 0;
//------------ Variables para controlar pluviometro ------------------------

volatile long int contadorPluv = 0;
int pulsosXhora = 0;
int pulsosXdia = 0;
float precipDIA = 0;
float precipHORA = 0;
long startTime = 0;  //para anti rebote.
long startTime2 = 0;    // para hacer una sola lectura de la lluvia.
long startTime3 = 0;    // para hacer una sola lectura de la lluvia.
time_t t;
//----------------------------------------------------------------------------


/**
 * @brief Setea si la hoja del campo esta mojada o seca
 * 
 * @param sensHoja valor leido del sensor
 * @return String result indicando si la hoja está mojada o seca
 */
String setHoja(int sensHoja) {
  String resul = "";
  if(sensHoja < 7){
    resul = "HOJA SECA";
  }
  if(sensHoja > 11){
    resul = "HOJA MOJADA";
  }
  return resul;
}

/**
 * @brief Setea la temperatura ambiente entre -25°C y 50°C
 * 
 * @param sensorTemp valor leido del sensor de temperatura
 * @return long int temp valor real de temperatura
 */
long int setTemperatura(int sensorTemp) {
  long int temp = 0;
  if(sensorTemp < 41){
    temp = -25;
  }
  if(sensorTemp > 41){
    temp = (((sensorTemp - 41) * 5 * 15) / 696) - 25;
  }
  return temp;
}

/**
 * @brief Setea la radiación solar entre 0 y 1400 W/m2
 * 
 * @param sensorRad valor leido del sensor de radiación solar
 * @return uint rad valor de radiacion solar
 */
unsigned long int setRadiacion(long int sensorRad) {
  long int rad = 0;
  if(sensorRad < 41){
    rad = 0;
  }
  if(sensorRad > 41){
     rad = ((sensorRad - 41) * 5 * 280) / 716;
  }
  return rad;
}

/**
 * @brief Setea la humedad del ambiente entre 0% y 100%
 * 
 * @param sensorHum valor del sensor de SENSOR_HUMEDAD
 * @return uint humedad porcentaje de SENSOR_HUMEDAD
 */
unsigned long int setHumedad(int sensorHum) {
  unsigned long int humedad = 0;
  if(sensorHum<102){
    humedad = 0;
  }
  if(sensorHum>102){
    humedad = ((sensorHum - 102) * 5 * 20) / 716;
  }
  return humedad;
}

/**
 * @brief Setea la dirección del viento
 * 
 * @param sensorDir valor del sensor
 * @return String dirViento que indica el nombre del punto cardinal donde apunta la dirección del viento
 */
String setDireccionViento(int sensorDir) {
  String dirViento = "";
  if(sensorDir>39 && sensorDir<103){
    dirViento = "NORTE";
  }
  if(sensorDir>715 && sensorDir<779){
    dirViento = "NORTE";
  }
  if(sensorDir>306 && sensorDir<431){
    dirViento = "SUR";
  }
  if(sensorDir>122 && sensorDir<247){
    dirViento = "ESTE";
  }
  if(sensorDir>490 && sensorDir<615){
    dirViento = "OESTE";
  }
  return dirViento;
}

/**
 * @brief Calcula la velocidad del viento. Convertimos a Km/h teniendo en cuenta que
 * como maximo llegan 4V (818)(240km/h)
 * 
 * @param sensorVel del sensor
 * @return long int valor de velocidad del viento
 */
long int setVelocidadViento(long int sensorVel) {
  long int vel=0;
   vel=(sensorVel * 5 * 48) / 818;
   return vel;
}

/**
 * @brief funcion ISR que aumenta el numero de pulsos al producirse
 * una interrupcion por flanco de subida en el pin.
 * Además debe indicar cuando se ha producido el primer conteo para que
 * se pueda calcular el tiempo de lluvia
 *
 */
void cuentaPulsos () {
  // TODO: agregar una activacion de un led para indicar que se ha producido una interrupcion e iniciar un contador de tiempo
  if(millis() - startTime2 > TIME_THRESHOLD){
    // Serial.println("Interrupcion");
    contadorPluv++;    
    startTime2 = millis();
  }
}

/**
 * @brief Contador de pulsos digitales para pluviometro
 * 
 * @param contador cantidad de pulsos digitales
 * @return int 
 */
float pluviometro (int contador) {
  // Un valor habitual de Paso es 0,25 mm lo cual significa que por cada 1 mm de
  // precipitación se producirán 4 vuelcos de cangilón
    return contador*0.25;
}

/**
 * @brief funcion para volver a cero el contador de pulsos del pluviometro
 * debe determinarse si hacerlo cada cierto tiempo o mediante una señal
 * externa de reset.
 * 
 */
void resetContadorPluv () {
  contadorPluv = 0;
}

/**
 * @brief Funcion que crea un Payload con los datos de los sensores
 *        en formato JSON.
 * 
 * @param contadorPluv
 * @param velViento
 * @param dirViento
 * @param humedad
 * @param radiacion
 * @param temperatura
 * @param hojaMojada
 * @param t
 * 
 * @return float humedad relativa
 */
String setPayload() {
  String jsonPayload = "{\"lluvia\":" + String(pluviometro(contadorPluv));
  jsonPayload += ",\"velocidadViento\":" + String(velViento);
  jsonPayload += ",\"direccionViento\":\"" + dirViento + "\"";
  jsonPayload += ",\"humedadRelativa\":" + String(humedad);
  jsonPayload += ",\"radiacionSolar\":" + String(radiacion);
  jsonPayload += ",\"temperatura\":" + String(temperatura);
  jsonPayload += ",\"hojaMojada\":" + String(hojaMojada);
  jsonPayload += ",\"tiempo\":" + String(t);
  jsonPayload += "}";

  resetContadorPluv();
  
  return jsonPayload;
}

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR_VEL_VIENTO, INPUT);
  pinMode(SENSOR_DIR_VIENTO, INPUT);
  pinMode(SENSOR_RADIACION, INPUT);
  pinMode(SENSOR_HUMEDAD, INPUT);
  pinMode(SENSOR_TEMPERATURA, INPUT);
  pinMode(SENSOR_HOJA, INPUT);
  pinMode(CELDA_1, INPUT);
  pinMode(CELDA_2, INPUT);
  pinMode(CELDA_3, INPUT);
  pinMode(CELDA_4, INPUT);

  attachInterrupt(digitalPinToInterrupt(SENSOR_PLUVIOMETRO), cuentaPulsos, RISING); // Interrupción por flanco de subida
}

void loop() {
  t = now();                                  // Declaramos la variable time_t 
  if (second(t)==58 && millis() - startTime2 > 1000) {
    
    Celda1 = analogRead(CELDA_1);             // lectura de las entradas analogicas Lisimetro 
    Celda2 = analogRead(CELDA_2);
    Celda3 = analogRead(CELDA_3);
    Celda4 = analogRead(CELDA_4);

    velViento = setVelocidadViento(analogRead(SENSOR_VEL_VIENTO));       //se leen las entradas analogicas Estación meteorológica.
    dirViento = setDireccionViento(analogRead(SENSOR_DIR_VIENTO));
    humedad = setHumedad(analogRead(SENSOR_HUMEDAD));
    radiacion = setRadiacion(analogRead(SENSOR_RADIACION));
    temperatura = setTemperatura(analogRead(SENSOR_TEMPERATURA));
    hojaMojada = setHoja(analogRead(SENSOR_HOJA));

    Serial.println("JSON GENERADO:");
    Serial.println(setPayload());
    startTime2 = millis();
  }
}
