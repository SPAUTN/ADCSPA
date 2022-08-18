#include <Time.h>
#include <TimeLib.h>

#define PIN_PLUVIOMETRO 18     // pin digital que soporta interrupciones (validos: 2,3,18,19,20,21)
#define TIME_THRESHOLD = 150

volatile long int contadorPluv = 0;
int pulsosXhora = 0;
int pulsosXdia = 0;
float precipDIA = 0;
float precipHORA = 0;
long startTime = 0;

void setup() {
  Serial.begin(9600);
  //Anotamos la hora y la fecha desde la que nuestro programa empezará a contar.
  //Formato: hora, minutos, segundos, días, mes, año
  setTime(23,59,40,31,8,2022);
  attachInterrupt(digitalPinToInterrupt(PIN_PLUVIOMETRO), cuentaPulsos, RISING); // Interrupción por flanco de subida
}

void loop() {  
  
  time_t t = now();//Declaramos la variable time_t 

  if (hour(t)==23 && minute(t)==59 && second(t)==59) {   // Para separar por dia.
    precipDIA = 0.25*pulsosXdia;
    Serial.print("Precipitaciones del dia: ");
    Serial.print(day(t));
    Serial.print(+ "/") ;
    Serial.print(month(t));
    Serial.print(+ "/") ;
    Serial.println(year(t)); 
    Serial.print(precipDIA);
    Serial.println(" mm");
    pulsosXdia = 0;   // reseteamos el acumulado del dia
  }

   // CUIDADO POSIBLE PROBLEMA DE CONCURRENCIA (le quitamos un segundo a las horas)
   
  if ( minute(t)==59 && second(t)==58) {   // Para separar por hora.
    pulsosXhora = contadorPluv;
    pulsosXdia =  pulsosXdia + pulsosXhora;
    precipHORA = 0.25 * pulsosXhora;
    Serial.print("Precipitaciones durante las: ");
    Serial.print(hour(t));  
    Serial.println(+ "hs") ;
    Serial.print(precipHORA);
    Serial.println(" mm/h");

    contadorPluv = 0;    //Reseteamos el contador de las interrupciones
  }
}

void cuentaPulsos(){
  if (millis() - startTime > TIME_THRESHOLD)   //anti rebote.
  {  
    contadorPluv++;
    startTime = millis();
  }
}
