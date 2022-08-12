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

void setup()
{
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

void loop()
{
  // se leen las entradas analogicas y se almacenan los valores en las variables
  Vviento = analogRead(SVviento);
  Dviento = analogRead(SDviento);
  Radiacion = analogRead(SRad);
  Temperatura = analogRead(STemperatura);
  Humedad = analogRead(SHumedad);
  HojaMojada = analogRead(SHMojada);
  Celda1 = analogRead(Ccarga1);
  Celda2 = analogRead(Ccarga2);
  Celda3 = analogRead(Ccarga3);
  Celda4 = analogRead(Ccarga4);

  float velocidadV = setVelocidadViento(Vviento);
  String direccionV = setDireccionViento(Dviento);
  // TODO: Lectura del valor de Radiacion
}

/**
 * Setea la dirección del viento
 */
String setDireccionViento(int direccion) {
  String direccionV = "";
  switch (direccion) {
  case (direccion > 39 && direccion< 103):
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
