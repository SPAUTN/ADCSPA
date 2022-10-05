
#include <Arduino.h>
#include <TimeLib.h>
#include "estacion.h"

extern volatile long int contadorPluv;;
extern unsigned long int velViento;
extern String dirViento;
extern unsigned long int radiacion;
extern long int temperatura;
extern long int presion;
extern unsigned long int humedad;
extern String hojaMojada;
extern time_t t;

/**
 * @brief Funcion que crea un Payload con los datos de los sensores
 *        en formato JSON.
 * 
 * @param contadorPluv
 * @param velViento
 * @param dirViento
 * @param humedad
 * @param radiacion
 * @param temperatura
 * @param hojaMojada
 * @param t
 * 
 * @return float humedad relativa
 */
String setPayload() {
  String jsonPayload = "{\"lluvia\":" + String(pluviometro(contadorPluv));
  jsonPayload += ",\"velocidadViento\":" + String(velViento);
  jsonPayload += ",\"hojaMojada\":\"" + String(hojaMojada)+ "\"";
  jsonPayload += ",\"direccionViento\":\"" + dirViento + "\"";
  jsonPayload += ",\"humedadRelativa\":" + String(humedad);
  jsonPayload += ",\"radiacionSolar\":" + String(radiacion);
  jsonPayload += ",\"temperatura\":" + String(temperatura);
  jsonPayload += ",\"presion\":" + String(presion);
  jsonPayload += ",\"tiempo\":" + String(t);
  jsonPayload += "}";

  resetContadorPluv();
  
  return jsonPayload;
}