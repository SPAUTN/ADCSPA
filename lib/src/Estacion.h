#include <SFE_BMP180.h>
#include <TimeLib.h>
#include "HX711.h"

#ifndef ESTACION_H
#define ESTACION_H
class Estacion {
    private:
        long initTime;
        long int velViento;
        int dirViento;
        unsigned long int humedad;
        unsigned long int radiacion;
        long int temperatura;
        long int presion;
        String humHoja;
        long contadorPluv;
        SFE_BMP180 bmp180;
        HX711 lisimetro;

        /**
         * @brief cambio de escala entre floats
         */
        static float fmap(float, float, float, float, float);
    public:
        Estacion(long);

        Estacion();

        // -------------------------------------- Inits --------------------------------------

        void init(SFE_BMP180);
        
        // -------------------------------------- Setters --------------------------------------
        
        /**
         * @brief Calcula la velocidad del viento. Convertimos a Km/h teniendo en cuenta que
         * como maximo llegan 4V (818)(240km/h)
         * 
         * @param sensorVel del sensor
         * @return long int valor de velocidad del viento
         */
        void setVelocidadViento(long int);
        
        /**
         * @brief Setea la dirección del viento
         * 
         * @param sensorDir valor del sensor
         * @return int valor de la dirección del viento en grados respecto al norte
         */
        void setDireccionViento(int);

        /**
         * @brief Setea la humedad del ambiente entre 0% y 100%
         * 
         * @param sensorHum valor del sensor de SENSOR_HUMEDAD
         * @return uint humedad porcentaje de SENSOR_HUMEDAD
         */
        void setHumedad(int);

        /**
         * @brief Setea la radiación solar entre 0 y 1400 W/m2
         * 
         * @param sensorRad valor leido del sensor de radiación solar
         * @return uint rad valor de radiacion solar
         */
        void setRadiacion(long int);

        /**
         * @brief Lectura de la temperatura del sensor bmp180
         */
        void setTemperatura();

        /**
         * @brief Lectura de la presion del sensor bmp180
         */
        void setPresion();

        /**
         * @brief Setea si la hoja del campo esta mojada o seca
         * 
         * @param sensHoja valor leido del sensor
         * @return String result indicando si la hoja está mojada o seca
         */
        void setHoja(int);

        /**
         * @brief funcion ISR que aumenta el numero de pulsos al producirse
         * una interrupcion por flanco de subida en el pin.
         * Además debe indicar cuando se ha producido el primer conteo para que
         * se pueda calcular el tiempo de lluvia
         */
        void cuentaPulsos (long int);

        /**
         * @brief funcion para volver a cero el contador de pulsos del pluviometro
         * debe determinarse si hacerlo cada cierto tiempo o mediante una señal
         * externa de reset.
         */
        void resetContadorPluv();
        
        // -------------------------------------- Getters --------------------------------------

        /**
         * @brief Get the Vel Viento object
         * 
         * @return long int 
         */
        long int getVelViento();

        /**
         * @brief Get the Dir Viento object
         * 
         * @return int 
         */
        int getDirViento ();

        /**
         * @brief Get the Humedad object
         * 
         * @return unsigned long int 
         */
        unsigned long int getHumedad();

        /**
         * @brief Get the Radiacion object
         * 
         * @return unsigned long int 
         */
        unsigned long int getRadiacion();

        /**
         * @brief Get the Temperatura object
         * 
         * @return long int 
         */
        long int getTemperatura();
        
        /**
         * @brief Get the Presion object
         * 
         * @return long int 
         */
        long int getPresion();
        
        /**
         * @brief Get the Hum Hoja object
         * 
         * @return String 
         */
        String getHumHoja();

        /**
         * @brief Get the Contador Pluv object
         * 
         * @return long 
         */
        long getContadorPluv();

        /**
         * @brief Get the Peso Lisimetro object
         * 
         * @return float 
         */
        float getPesoLisimetro();

        /**
         * @brief Get the Temp Modulo object
         * 
         * @return SFE_BMP180 
         */
        SFE_BMP180 getTempModulo();
};
#endif
