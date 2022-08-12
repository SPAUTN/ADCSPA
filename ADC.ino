//Este algoritmo tiene 2 objetivos principales
// 1- Tomar las señales analogicas de la Estación meteorológica y convertirlas a digitales con la mayor resolución posible
// 2- Tomar las señales analogicas del Lisímetro de pesada y convertirlas a digitales con la mayor resolución posible
//A0 – Sensor velocidad del viento (0V -4 V)
//A1 - Sensor dirección del viento (0.2V – 3.8V)
//A2 – Sensor de radiación (0V – 3V)
//A3 – Sensor de humedad (0.5V – 4V)
//A4 – Sensor de temperatura (0.2V – 3.6V)
//A5 – Sensor de hoja mojada (seca<20mV – mojada>50mV)
//A6 – Celda de carga 1
//A7 – Celda de carga 2
//A8 – Celda de carga 3
//A9 – Celda de carga 4
//Pluviometro - cualquier entrada digital (cable Normalmente cerrado del reed swicth) (pin 50)

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
//-------------------------------------------------------------------------
//------------- Definición entradas digitales -----------------------------
#define SLluvia 50
//-------------------------------------------------------------------------
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
//-------------------------------------------------------------------------

void setup() 
{
  Serial.begin(9600);            //iniciar puerto serie
  pinMode(SVviento, INPUT);      //Entrada analógica
  pinMode(SDviento, INPUT);      //Entrada analógica
  pinMode(SRad, INPUT);          //Entrada analógica
  pinMode(SHumedad, INPUT);      //Entrada analógica
  pinMode(STemperatura, INPUT);  //Entrada analógica
  pinMode(SHMojada, INPUT);      //Entrada analógica
  pinMode(Ccarga1, INPUT);       //Entrada analógica
  pinMode(Ccarga2, INPUT);       //Entrada analógica
  pinMode(Ccarga3, INPUT);       //Entrada analógica
  pinMode(Ccarga4, INPUT);       //Entrada analógica
  pinMode(SLluvia, INPUT);       //Entrada digital

}

void loop() 
{
  float velocidadV=0;
  String direccionV = "";  
   
//se leen las entradas analogicas y se almacenan los valores en las variables
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
  
//------- leer entrada digital del pluviometro-----------------
//
//--------------------------------------------------------------
 
//En esta sección convertimos a las unidades correspondientes de cada sensor

  velocidadV = (Vviento*5*48)/818;       //Convertimos a Km/h teniendo en cuenta que como maximo llegan 4V (818)(240km/h)

  switch(Dviento)                        //Determinamos la direccíon del viento(0,2V - 3,8V)
                {
                 case (Dviento>39 && Dviento<103):
                 direccionV = "NORTE";
                 break;
                 case (Dviento>715 && Dviento<779):
                 direccionV = "NORTE";
                 break;
                 case (Dviento>306 && Dviento<431):
                 direccionV = "SUR";
                 break;
                 case (Dviento>122 && Dviento<247):
                 direccionV = "ESTE";
                 break;
                 case (Dviento>490 && Dviento<615):
                 direccionV = "OESTE";
                 break;
                }
                
}

  

