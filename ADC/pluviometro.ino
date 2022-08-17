#include <Time.h>
#include <TimeLib.h>

#define SLluvia 18     // pin digital que soporta interrupciones (validos: 2,3,18,19,20,21)

const int timeThreshold = 150;
volatile int contadorPluv = 0;
int pulsosXhora = 0;
int pulsosXdia = 0;
float precipDIA = 0;
float precipHORA = 0;
long startTime = 0;

void setup()
{
  Serial.begin(9600);
  //Anotamos la hora y la fecha desde la que nuestro programa empezará a contar.
  //Formato: hora, minutos, segundos, días, mes, año
  setTime(23,59,40,17,8,2022);
  attachInterrupt(digitalPinToInterrupt(SLluvia), cuentaPulsos, RISING); // Interrupción por flanco de subida
}

void loop()
{  
     time_t t = now();//Declaramos la variable time_t 

  if (hour(t)==23 && minute(t)==59 && second(t)==59){   // Para separar por dia.
  pulsosXdia = contadorPluv;
  precipDIA = 0.25*pulsosXdia;
  Serial.print("Precipitaciones del dia: ");
  Serial.print(day(t));
  Serial.print(+ "/") ;
  Serial.print(month(t));
  Serial.print(+ "/") ;
  Serial.println(year(t)); 
  Serial.print(precipDIA);
  Serial.println(" mm");
  
  contadorPluv = 0;    //Reseteamos el contador de las interrupciones cada 24hs.
  }
  if ( minute(t)==59 && second(t)==59){   // Para separar por hora.
  pulsosXhora = contadorPluv;
  precipHORA = 0.25*contadorPluv;
  Serial.print("Precipitaciones a las: ");
  Serial.print(hour(t));  
  Serial.print(+ ":") ;
  Serial.print(minute(t));
  Serial.print(":") ;
  Serial.println(second(t));
  Serial.print(precipHORA);
  Serial.println(" mm/h");
  }
}

void cuentaPulsos(){
  if (millis() - startTime > timeThreshold)   //anti rebote.
  {  
    contadorPluv++;
    startTime = millis();
  }
}
