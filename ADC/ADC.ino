#include <Time.h>
#include <TimeLib.h>

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
#define PIN_PLUVIOMETRO 18     // pin digital que soporta interrupciones (validos: 2,3,18,19,20,21)

//------------ Variables para almacenar los datos -------------------------
unsigned long int Vviento = 0;
String direccionV = "";
unsigned long int Radiacion = 0;
long int Temperatura = 0;
unsigned long int Humedad = 0;
String hojaMojada = "";
int Celda1 = 0;
int Celda2 = 0;
int Celda3 = 0;
int Celda4 = 0;
//------------ Variables para controlar pluviometro ------------------------
#define TIME_THRESHOLD 150     // para anti rebote de reed switch
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
  Serial.begin(9600);           // iniciar puerto serie
  //Anotamos la hora y la fecha desde la que nuestro programa empezará a contar.
  //Formato: hora, minutos, segundos, días, mes, año
  setTime(23,59,30,19,8,2022);
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
  attachInterrupt(digitalPinToInterrupt(PIN_PLUVIOMETRO), cuentaPulsos, RISING); // Interrupción por flanco de subida
}

void loop() {
  t = now();                                //Declaramos la variable time_t 
  if ( minute(t)==59 && second(t)==58) {    // Tomamos los datos cada 1 hora.
  
  Celda1 = analogRead(Ccarga1);             // se leen las entradas analogicas Lisimeto 
  Celda2 = analogRead(Ccarga2);
  Celda3 = analogRead(Ccarga3);
  Celda4 = analogRead(Ccarga4);

  Vviento = setVelocidadViento(analogRead(SVviento));       //se leen las entradas analogicas Estación meteorológica.
  direccionV = setDireccionViento(analogRead(SDviento));
  Humedad = setHumedad(analogRead(SHumedad));
  Radiacion = setRadiacion(analogRead(SRad));
  Temperatura = setTemperatura(analogRead(STemperatura));
  hojaMojada = setHoja(analogRead(SHMojada));
  setLluvia();

  mostrarDatos();     // imprime por puerto serie los valores de los sensores analogicos.
   }

    if (hour(t)==23 && minute(t)==59 && second(t)==59) {   // Para separar por dia.
    if (millis() - startTime3 > 1000){                     // Lo coloque aca porque dentro del IF que separa por horas
        precipDIA = 0.25*pulsosXdia;                       // nunca se ejecutaría
        Serial.print("Precipitaciones del dia: ");
        Serial.print(day(t));
        Serial.print(+ "/") ;
        Serial.print(month(t));
        Serial.print(+ "/") ;
        Serial.println(year(t)); 
        Serial.print(precipDIA);
        Serial.println(" mm");
        startTime3 = millis();
        pulsosXdia = 0;   // reseteamos el acumulado del dia
    }
  }
}

/**
 * imprime por puerto serie los valores de los sensores analogicos.
 */
void mostrarDatos(){
    Serial.print("Velocidad del viento: ");     //ok 
    Serial.print(Vviento);
    Serial.println("Km/h");
    
    Serial.print("Dirección del viento:");
    Serial.println(direccionV);  //ok
    
    Serial.print("Humedad:");
    Serial.print(Humedad);     //ok
    Serial.println("%");

    Serial.print("Radiación:");
    Serial.print(Radiacion); //ok
    Serial.println("W/m2");

    Serial.print("Temperatura:");
    Serial.print(Temperatura);  //ok
    Serial.println("°C");
    Serial.println(hojaMojada);   //ok
  delay(1000);
}


/**
 * Setea las precipitaciones por hora y por dia.
 */
void setLluvia(){

   // CUIDADO POSIBLE PROBLEMA DE CONCURRENCIA (le quitamos un segundo a las horas)
   
  if ( minute(t)==59 && second(t)==58) {   // Para separar por hora.
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
}

/**
 *Rutina de interrupción para el pluviometro.
 */
void cuentaPulsos(){
  if (millis() - startTime > TIME_THRESHOLD)   //anti rebote.
  {  
    contadorPluv++;
    startTime = millis();
  }
}

/**
 * Setea si la hoja del campo esta mojada o seca
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
 * Setea la temperatura ambiente entre -25°C y 50°C
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
 * Setea la radiación solar entre 0 y 1400 W/m2
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
 * Setea la humedad del ambiente entre 0% y 100%
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
 * Setea la dirección del viento
 */
String setDireccionViento(int direccion) {
  String direccionViento = "";
  if(direccion>39 && direccion<103){
    direccionViento = "NORTE";
  }
  if(direccion>715 && direccion<779){
    direccionViento = "NORTE";
  }
  if(direccion>306 && direccion<431){
    direccionViento = "SUR";
  }
  if(direccion>122 && direccion<247){
    direccionViento = "ESTE";
  }
  if(direccion>490 && direccion<615){
    direccionViento = "OESTE";
  }
  return direccionViento;
}

/**
 * Convertimos a Km/h teniendo en cuenta que como maximo llegan 4V (818)(240km/h)
 */
long int setVelocidadViento(long int sensorVel) {
  long int vel=0;
  vel=(sensorVel * 5 * 48) / 818;
  return vel;
}


