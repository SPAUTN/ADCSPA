#include <Arduino.h>
#include <TimeLib.h>
#include "Estacion.hpp"

extern time_t t;

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
 * @param t
 * 
 * @return float humedad relativa
 */
String setPayload(Estacion estacion) {

  float test = 1.21 * 1.26;

  String jsonPayload = "{";
  
  jsonPayload += "\"lluvia\":" + String(estacion.getContadorPluv());
  jsonPayload += ",\"velocidadViento\":" + String(estacion.getVelViento());
  jsonPayload += ",\"hojaMojada\":\"" + String(estacion.getHumHoja())+ "\"";
  jsonPayload += ",\"direccionViento\":" + String(estacion.getDirViento());
  jsonPayload += ",\"humedadRelativa\":" + String(estacion.getHumedad());
  jsonPayload += ",\"radiacionSolar\":" + String(estacion.getRadiacion());
  jsonPayload += ",\"temperatura\":" + String(estacion.getTemperatura());
  jsonPayload += ",\"presion\":" + String(estacion.getPresion());
  jsonPayload += ",\"tiempo\":" + String(t);
  jsonPayload += ",\"prueba\":" + String(test, 4);
  
  jsonPayload += "}";

  estacion.resetContadorPluv();
  
  return jsonPayload;
}