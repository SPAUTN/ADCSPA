#include <SFE_BMP180.h>
#include <TimeLib.h>

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
    public:
        Estacion(long);
        Estacion();
        // -------------------------------------- Setters --------------------------------------
        void setVelocidadViento(long int);
        void setDireccionViento(int);
        static float fmap(float, float, float, float, float);
        void setHumedad(int);
        void setRadiacion(long int);
        void setTemperatura(SFE_BMP180);
        void setPresion(SFE_BMP180);
        void setHoja(int);
        void cuentaPulsos (long int);
        void resetContadorPluv();

        // -------------------------------------- Getters --------------------------------------
        long int getVelViento();
        int getDirViento ();
        unsigned long int getHumedad();
        unsigned long int getRadiacion();
        long int getTemperatura();
        long int getPresion();
        String getHumHoja();
        long getContadorPluv();
};
#endif
