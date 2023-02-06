
#include <Arduino.h>
#include <TimeLib.h>
#include "estacion.h"

extern volatile long int contadorPluv;
extern unsigned long int velViento;
extern int dirViento;
extern unsigned long int radiacion;
extern long int temperatura;
extern long int presion;
extern unsigned long int humedad;
extern String hojaMojada;
extern time_t t;
extern float peso;

/**
 * @brief Funcion que crea un Payload con los datos de los sensores
 *        en formato JSON.
 * 
 * @param pluviometro(contadorPluv) - mm de lluvia acumulada 
 * @param velViento - Velocidad del viento en km/h
 * @param dirViento - Direccion del viento en grados respecto al norte
 * @param humedad - Porcentaje de humedad
 * @param radiacion - Radiacion solar en W/m^2
 * @param temperatura - Temperatura en grados centigrados
 * @param hojaMojada - Estado de la hoja de la planta
 * @param peso - Peso del lisimetro en Kg
 * @param t
 * 
 * @return float humedad relativa
 */
String setPayload() {
  String jsonPayload = "{";
  
  jsonPayload += "\"lluvia\":" + String(pluviometro(contadorPluv));
  jsonPayload += ",\"velocidadViento\":" + String(velViento);
  jsonPayload += ",\"hojaMojada\":\"" + String(hojaMojada)+ "\"";
  jsonPayload += ",\"direccionViento\":" + String(dirViento);
  jsonPayload += ",\"humedadRelativa\":" + String(humedad);
  jsonPayload += ",\"radiacionSolar\":" + String(radiacion);
  jsonPayload += ",\"temperatura\":" + String(temperatura);
  jsonPayload += ",\"presion\":" + String(presion);
  jsonPayload += ",\"tiempo\":" + String(t);
  jsonPayload += ",\"peso\":" + String(peso);
  
  jsonPayload += "}";

  resetContadorPluv();
  
  return jsonPayload;
}