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
#define SLluvia 50

//------------ Variables para almacenar los datos -------------------------
int Vviento = 0;
int Dviento = 0;
int Radiacion = 0;
int Temperatura = 0;
int HojaMojada = 0;
int Humedad = 0;
int Lluvia = 0;
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
  pinMode(SLluvia, INPUT);      // Entrada digital
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
}

/**
 * Setea si la hoja del campo esta mojada o seca
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
 * Setea la temperatura ambiente entre -25°C y 50°C
 */
int setTemperatura(int T) {
  float temp = 0;
  switch (T)
  {
  case (T < 41):
    temp = -25;
    break;
  case (T < 41):
    temp = (((T - 41) * 5 * 15) / 696) - 25;
    break;
  }
  return temp;
}

/**
 * Setea la radiación solar entre 0 y 1400 W/m2
 */
int setRadiacion(int R) {
  float rad = 0;
  switch (R)
  {
  case (R < 41):
    rad = 0;
    break;
  case (R > 41):
    rad = ((R - 41) * 5 * 280) / 716;
    break;
  }
  return rad;
}

/**
 * Setea la humedad del ambiente entre 0% y 100%
 */
int setHumedad(int H) {
  float humedad = 0;
  switch (H)
  {
  case (H < 102):
    humedad = 0;
    break case (H > 102) : humedad = ((H - 102) * 5 * 20) / 716 break;
  }
  return humedad;
}

/**
 * Setea la dirección del viento
 */
String setDireccionViento(int direccion) {
  String direccionV = "";
  switch (direccion)
  {
  case (direccion > 39 && direccion < 103):
    direccionV = "NORTE";
    break;
  case (direccion > 715 && direccion < 779):
    direccionV = "NORTE";
    break;
  case (direccion > 306 && direccion < 431):
    direccionV = "SUR";
    break;
  case (direccion > 122 && direccion < 247):
    direccionV = "ESTE";
    break;
  case (direccion > 490 && direccion < 615):
    direccionV = "OESTE";
    break;
  }
  return direccionV;
}

/**
 * Convertimos a Km/h teniendo en cuenta que como maximo llegan 4V (818)(240km/h)
 */
int setVelocidadViento(int lectura) {
  return (lectura * 5 * 48) / 818;
}
