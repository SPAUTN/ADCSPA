#include "HX711.h"

int suma=0; //suma del peso de las 4 celdas
int promedio=0; //promedio del peso de las 4 celdas
//celda 1
const int DOUT1=A1;
const int CLK1=A0;
//celda2
const int DOUT2=A3;
const int CLK2=A2;
//celda3
const int DOUT3=A5;
const int CLK3=A4;
//celda4
const int DOUT4=A7;
const int CLK4=A6;

HX711 celda1;
HX711 celda2;
HX711 celda3;
HX711 celda4;

void setup() {
  Serial.begin(9600);
  
  celda1.begin(DOUT1, CLK1);
  Serial.print("Lectura del valor del ADC1:  ");
  Serial.println(celda1.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  celda1.set_scale(); //valor obtenido en calibración. Establecemos la escala
  celda1.tare(20);  //El peso actual es considerado Tara.
  
  celda1.begin(DOUT2, CLK2);
  Serial.print("Lectura del valor del ADC2:  ");
  Serial.println(celda2.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  celda2.set_scale(); //valor obtenido en calibración. Establecemos la escala
  celda2.tare(20);  //El peso actual es considerado Tara.
    
  celda1.begin(DOUT3, CLK3);
  Serial.print("Lectura del valor del ADC3:  ");
  Serial.println(celda3.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  celda3.set_scale(); //valor obtenido en calibración. Establecemos la escala
  celda3.tare(20);  //El peso actual es considerado Tara.
    
  celda1.begin(DOUT4, CLK4);
  Serial.print("Lectura del valor del ADC4:  ");
  Serial.println(celda4.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  celda4.set_scale(); //valor obtenido en calibración. Establecemos la escala
  celda4.tare(20);  //El peso actual es considerado Tara.
  
  Serial.println("Listo para pesar");  
}

void loop() {
  Serial.print("Peso celda 1: ");
  Serial.print(celda1.get_units(20),3);
  Serial.println(" kg");
  delay(500);
  
  Serial.print("Peso celda 2: ");
  Serial.print(celda2.get_units(20),3);
  Serial.println(" kg");
  delay(500);
  
  Serial.print("Peso celda 3: ");
  Serial.print(celda3.get_units(20),3);
  Serial.println(" kg");
  delay(500);
  
  Serial.print("Peso celda 4: ");
  Serial.print(celda4.get_units(20),3);
  Serial.println(" kg");
  delay(500);

  suma = ((celda1.get_units(20),3)+(celda2.get_units(20),3)+(celda3.get_units(20),3)+(celda4.get_units(20),3));
  promedio= suma/4;
  Serial.print("Peso del lisimetro: ");
  Serial.print(promedio);
  Serial.print(" kg");
}
