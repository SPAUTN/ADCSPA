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
unsigned long int Vviento = 0;
String direccionV = "";
unsigned long int radiacion = 0;
long int Temperatura = 0;
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

    Vviento = setVelocidadViento(analogRead(SENSOR_VEL_VIENTO));       //se leen las entradas analogicas Estación meteorológica.
    direccionV = setDireccionViento(analogRead(SENSOR_DIR_VIENTO));
    humedad = setHumedad(analogRead(SENSOR_HUMEDAD));
    radiacion = setRadiacion(analogRead(radiacion));
    Temperatura = setTemperatura(analogRead(SENSOR_TEMPERATURA));
    hojaMojada = setHoja(analogRead(SENSOR_HOJA));

    // setLluvia();

    // mostrarDatos();     // imprime por puerto serie los valores de los sensores analogicos.

    Serial.println("JSON GENERADO:");
    Serial.println(setPayload());
    startTime2 = millis();
  }
}

/**
* @brief Función mostrar los datos leidos y calculados en el arduino.
* Cada 1 segudo, por el momento, dado como valor fijo
*/
void mostrarDatos(){
    Serial.print("Velocidad del viento: ");     //ok 
    Serial.print(Vviento);
    Serial.println("Km/h");
    
    Serial.print("Dirección del viento:");
    Serial.println(direccionV);  //ok
    
    Serial.print("SENSOR_HUMEDAD:");
    Serial.print(humedad);     //ok
    Serial.println("%");

    Serial.print("Radiación:");
    Serial.print(radiacion); //ok
    Serial.println("W/m2");

    Serial.print("Temperatura:");
    Serial.print(Temperatura);  //ok
    Serial.println("°C");
    Serial.println(hojaMojada);   //ok
  delay(1000);
}

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
long int setRadiacion(long int sensorRad) {
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
 * @return String direccionV que indica el nombre del punto cardinal donde apunta la dirección del viento
 */
String setDireccionViento(int sensorDir) {
  String direccionV = "";
  if(sensorDir>39 && sensorDir<103){
    direccionV = "NORTE";
  }
  if(sensorDir>715 && sensorDir<779){
    direccionV = "NORTE";
  }
  if(sensorDir>306 && sensorDir<431){
    direccionV = "SUR";
  }
  if(sensorDir>122 && sensorDir<247){
    direccionV = "ESTE";
  }
  if(sensorDir>490 && sensorDir<615){
    direccionV = "OESTE";
  }
  return direccionV;
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
    Serial.println("Interrupcion");
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
int pluviometro (int contador) {
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

void setLluvia(){
    if (millis() - startTime2 > 1000){
        pulsosXhora = contadorPluv;
        pulsosXdia =  pulsosXdia + pulsosXhora;
        precipHORA = 0.25 * pulsosXhora;
        Serial.print("Precipitaciones durante las: ");
        Serial.print(hour(t));  
        Serial.println(+ "hs") ;
        Serial.print(precipHORA);
        Serial.println(" mm/h");
        startTime2 = millis();
        contadorPluv = 0;    //Reseteamos el contador de las interrupciones
    }
}
/**
 * @brief Funcion que crea un Payload con los datos de los sensores
 *        en formato JSON.
 * 
 * @param contadorPluv
 * @param Vviento
 * @param direccionV
 * @param humedad
 * @param radiacion
 * @param Temperatura
 * @param hojaMojada
 * @param t
 * 
 * @return float humedad relativa
 */
String setPayload() {
  String jsonPayload = "{\"lluvia\":" + String(contadorPluv * 0.25);
  jsonPayload += ",\"velocidadViento\":" + String(Vviento);
  jsonPayload += ",\"direccionViento\":\"" + direccionV + "\"";
  jsonPayload += ",\"humedadRelativa\":" + String(humedad);
  jsonPayload += ",\"radiacionSolar\":" + String(radiacion);
  jsonPayload += ",\"temperatura\":" + String(Temperatura);
  jsonPayload += ",\"hojaMojada\":" + String(hojaMojada);
  jsonPayload += ",\"tiempo\":" + String(t);
  jsonPayload += "}";

  resetContadorPluv();
  
  return jsonPayload;
}
