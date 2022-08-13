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
int Vviento = 0;
int Dviento = 0;
int Radiacion = 0;
int Temperatura = 0;
int HojaMojada = 0;
int Humedad = 0;
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

  float velocidadV = setVelocidadViento(analogRead(SVviento));
  String direccionV = setDireccionViento(analogRead(SDviento));
  float Hum = setHumedad(analogRead(SHumedad));
  float radiacion = setRadiacion(analogRead(SRad));
  float temperatura = setTemperatura(analogRead(STemperatura));
  String hojaMojada = setHoja(analogRead(SHMojada));
  int lluvia = setLluvia(digitalRead(SLluvia));
}

/**
 * @brief Setea si la hoja del campo esta mojada o seca
 * 
 * @param Hoja 
 * @return String result indicando si la hoja está mojada o seca
 */
String setHoja(int Hoja) {
  String resul = "";
  switch (Hoja)
  {
  case (Hoja < 7):
    resul = "HOJA SECA";
    break;
  case (Hoja > 11):
    resul = "HOJA MOJADA";
    break;
  }
  return resul;
}

/**
 * @brief Setea la temperatura ambiente entre -25°C y 50°C
 * 
 * @param sensorTemp valor leido del sensor
 * @return int temp valor real de temperatura
 */
int setTemperatura(int sensorTemp) {
  float temp = 0;
  switch (sensorTemp)
  {
  case (sensorTemp < 41):
    temp = -25;
    break;
  case (sensorTemp < 41):
    temp = (((T - 41) * 5 * 15) / 696) - 25;
    break;
  }
  return temp;
}

/**
 * @brief Setea la radiación solar entre 0 y 1400 W/m2
 * 
 * @param sensorRad valor leido del sensor de radiación solar
 * @return int rad valor de radiacion solar
 */
int setRadiacion(int sensorRad) {
  float rad = 0;
  switch (sensorRad)
  {
  case (sensorRad < 41):
    rad = 0;
    break;
  case (sensorRad > 41):
    rad = ((sensorRad - 41) * 5 * 280) / 716;
    break;
  }
  return rad;
}

/**
 * @brief Setea la humedad del ambiente entre 0% y 100%
 * 
 * @param sensorHum valor del sensor de humedad
 * @return int humedad porcentaje de humedad
 */
int setHumedad(int sensorHum) {
  float humedad = 0;
  switch (sensorHum)
  {
  case (sensorHum < 102):
    humedad = 0;
    break;
  case (sensorHum > 102) :
    humedad = ((sensorHum - 102) * 5 * 20) / 716
    break;
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
  switch (sensorDir)
  {
  case (sensorDir > 39 && sensorDir < 103):
    direccionV = "NORTE";
    break;
  case (sensorDir > 715 && sensorDir < 779):
    direccionV = "NORTE";
    break;
  case (sensorDir > 306 && sensorDir < 431):
    direccionV = "SUR";
    break;
  case (sensorDir > 122 && sensorDir < 247):
    direccionV = "ESTE";
    break;
  case (sensorDir > 490 && sensorDir < 615):
    direccionV = "OESTE";
    break;
  }
  return direccionV;
}

/**
 * @brief Calcula la velocidad del viento. Convertimos a Km/h teniendo en cuenta que
 * como maximo llegan 4V (818)(240km/h)
 * 
 * @param sensorVel del sensor
 * @return int valor de velocidad del viento
 */
int setVelocidadViento(int sensorVel) {
  return (sensorVel * 5 * 48) / 818;
}

/**
 * @brief funcion ISR que aumenta el numero de pulsos al producirse
 * una interrupcion por flanco de subida en el pin 
 *
 */
void cuentaPulsos () {
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