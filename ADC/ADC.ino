//-------------- Definición de entradas analogicas -----------------------
#define SVviento A0
#define SDviento A1
#define SRad A2
#define SHumedad A3
#define STemperatura A4
#define SHMojada A5
#define Ccarga1 A6
#define Ccarga2 A7
#define Ccarga3 A8
#define Ccarga4 A9

//------------- Definición entradas digitales -----------------------------
#define SLluvia 50 // TODO debe ser un pin de interrupción

//------------ Variables para almacenar los datos -------------------------
unsigned long int Vviento = 0;
unsigned long int Radiacion = 0;
long int Temperatura = 0;
unsigned long int Humedad = 0;
int contadorPluv = 0;
int Celda1 = 0;
int Celda2 = 0;
int Celda3 = 0;
int Celda4 = 0;

void setup() {
  Serial.begin(9600);           // iniciar puerto serie
  pinMode(SVviento, INPUT);     // Entrada analógica
  pinMode(SDviento, INPUT);     // Entrada analógica
  pinMode(SRad, INPUT);         // Entrada analógica
  pinMode(SHumedad, INPUT);     // Entrada analógica
  pinMode(STemperatura, INPUT); // Entrada analógica
  pinMode(SHMojada, INPUT);     // Entrada analógica
  pinMode(Ccarga1, INPUT);      // Entrada analógica
  pinMode(Ccarga2, INPUT);      // Entrada analógica
  pinMode(Ccarga3, INPUT);      // Entrada analógica
  pinMode(Ccarga4, INPUT);      // Entrada analógica
  attachInterrupt(digitalPinToInterrupt(SLluvia), cuentaPulsos, RISING); // Interrupción por flanco de subida
}

void loop() {
  // se leen las entradas analogicas y se almacenan los valores en las variables
  Celda1 = analogRead(Ccarga1);
  Celda2 = analogRead(Ccarga2);
  Celda3 = analogRead(Ccarga3);
  Celda4 = analogRead(Ccarga4);

  Vviento = setVelocidadViento(analogRead(SVviento));
  String direccionV = setDireccionViento(analogRead(SDviento));
  Humedad = setHumedad(analogRead(SHumedad));
  Radiacion = setRadiacion(analogRead(SRad));
  Temperatura = setTemperatura(analogRead(STemperatura));
  String hojaMojada = setHoja(analogRead(SHMojada));
  int lluvia = setLluvia(digitalRead(SLluvia));
}

/**
 * @brief Setea si la hoja del campo esta mojada o seca
 * 
 * @param sensHoja 
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
 * @param sensorTemp valor leido del sensor
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
 * @param sensorHum valor del sensor de humedad
 * @return uint humedad porcentaje de humedad
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
  contadorPluv++;
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
