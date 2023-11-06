#include <SFE_BMP180.h>
#include <TimeLib.h>
#include "HX711.h"
#include "DHT.h"
#include <SFE_BMP180.h>
#include "DHT.h"

#define ATTEMPTS 5

#ifndef ESTACION_H
#define ESTACION_H
class WeatherStation {
    private:
        long initTime;
        long int windSpeed;
        int windDirection;
        float humidity;
        unsigned long int radiation;
        long int temperature;
        long int pressure;
        int leafMoisture;
        long pluviometerCounter;
        HX711 lysimeter;
        SFE_BMP180 bmp180Sensor;
        DHT dht;

        /**
         * @brief cambio de escala entre floats
         */
        float fmap(float, float, float, float, float);

        /**
         * @brief funcion para volver a cero el contador de pulsos del pluviometro
         * debe determinarse si hacerlo cada cierto tiempo o mediante una señal
         * externa de reset.
         */
        void resetPulseCounter();


    public:
        WeatherStation(long);

        WeatherStation();

        // -------------------------------------- Inits --------------------------------------

        /**
         * @brief Inicialización de los objetos bmp180Sensor y lisímetro
         * 
         */
        void init();
        
        // -------------------------------------- Setters --------------------------------------
        
        /**
         * @brief Calcula la velocidad del viento. Convertimos a Km/h teniendo en cuenta que
         * como maximo llegan 4V (818)(240km/h)
         * 
         * @param sensorVel del sensor
         * @return long int valor de velocidad del viento
         */
        void setWindSpeed(long int);
        
        /**
         * @brief Setea la dirección del viento
         * 
         * @param sensorDir valor del sensor
         * @return int valor de la dirección del viento en grados respecto al norte
         */
        void setwindDirection(int);

        /**
         * @brief Setea la humedad del ambiente entre 0% y 100%
         * 
         * @param sensorHum valor del sensor de SENSOR_HUMEDAD
         * @return uint humedad porcentaje de SENSOR_HUMEDAD
         */
        void setHumidity(int);

        /**
         * @brief Setea la radiación solar entre 0 y 1400 W/m2
         * 
         * @param sensorRad valor leido del sensor de radiación solar
         * @return uint rad valor de radiacion solar
         */
        void setRadiation(long int);

        /**
         * @brief Lectura de la temperatura del sensor bmp180Sensor
         */
        void setTemperature();

        /**
         * @brief Lectura de la presion del sensor bmp180Sensor
         */
        void setPresion();

        /**
         * @brief Setea si la hoja del campo esta mojada o seca
         * 
         * @param sensHoja valor leido del sensor
         * @return String result indicando si la hoja está mojada o seca
         */
        void setLeafMoisture(int);

        /**
         * @brief funcion ISR que aumenta el numero de pulsos al producirse
         * una interrupcion por flanco de subida en el pin.
         * Además debe indicar cuando se ha producido el primer conteo para que
         * se pueda calcular el tiempo de lluvia
         */
        void setPulseCounter (long int);
        
        /**
         * @brief Función que calcula el riego, y lo ejecuta
         * 
         * @param 
         * @return 
         */
        float irrigateAndGetETc(float, float);
        // -------------------------------------- Getters --------------------------------------

        /**
         * @brief Get the Vel Viento object
         * 
         * @return long int 
         */
        long int getWindSpeed();

        /**
         * @brief Get the Dir Viento object
         * 
         * @return int 
         */
        int getWindDirection();

        /**
         * @brief Get the Humedad object
         * 
         * @return unsigned long int 
         */
        float getHumidity();

        /**
         * @brief Get the Radiacion object
         * 
         * @return unsigned long int 
         */
        unsigned long int getRadiation();

        /**
         * @brief Get the Temperatura object
         * 
         * @return long int 
         */
        long int getTemperature();
        
        /**
         * @brief Get the Presion object
         * 
         * @return long int 
         */
        long int getPressure();
        
        /**
         * @brief Get the Hum Hoja object
         * 
         * @return int with percentage of humidity in the leaf
         */
        int getLeafMoisture();

        /**
         * @brief Get the Contador Pluv object
         * 
         * @return long 
         */
        float getPluviometerCounter();

        /**
         * @brief Get the Peso Lisimetro object
         * 
         * @return float 
         */
        float getLysimeterWeight();

        /**
         * @brief Retorna el payload generado con los valores de la estación
         * 
         */
        String getPayload();

};
#endif
