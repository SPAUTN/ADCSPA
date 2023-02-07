#include "unity.h"
#include "Estacion.h"

#ifdef UNIT_TEST

Estacion estacion = Estacion(0);


void testSetHumedad() {

    Estacion estacion = Estacion(0);
    // Test valores negativos y menores a 102
    estacion.setHumedad(0);
    TEST_ASSERT_EQUAL(0, estacion.getHumedad());
    estacion.setHumedad(-10);
    TEST_ASSERT_EQUAL(0, estacion.getHumedad());
    estacion.setHumedad(-20);
    TEST_ASSERT_EQUAL(0, estacion.getHumedad());
    estacion.setHumedad(100);
    TEST_ASSERT_EQUAL(0, estacion.getHumedad());
    estacion.setHumedad(102);
    TEST_ASSERT_EQUAL(0, estacion.getHumedad());

    // Test valores mayores a 102
    estacion.setHumedad(103);
    TEST_ASSERT_EQUAL( 0.14, estacion.getHumedad());
    estacion.setHumedad(110);
    TEST_ASSERT_EQUAL( ((110 - 102) * 5 * 20) / 716, estacion.getHumedad());
    estacion.setHumedad(120);
    TEST_ASSERT_EQUAL( ((120 - 102) * 5 * 20) / 716, estacion.getHumedad());

    // Valores específicos de humedad
    // Humedad 25%
    estacion.setHumedad(281);
    TEST_ASSERT_EQUAL(25, estacion.getHumedad());
    // Humedad 40%
    estacion.setHumedad(389);
    TEST_ASSERT_EQUAL(40, estacion.getHumedad());
    // Humedad 45%
    estacion.setHumedad(425);
    TEST_ASSERT_EQUAL(45, estacion.getHumedad());
        // Humedad 47%
        estacion.setHumedad(439);
    TEST_ASSERT_EQUAL(47, estacion.getHumedad());
    // Humedad de 50%
    estacion.setHumedad(460);
    TEST_ASSERT_EQUAL(50, estacion.getHumedad());
    // Humedad de 75%
    estacion.setHumedad(639);
    TEST_ASSERT_EQUAL(75, estacion.getHumedad());
    // Humedad de 100%
    estacion.setHumedad(818);
    TEST_ASSERT_EQUAL(100, estacion.getHumedad());

    // Test valor mayor a 818
    estacion.setHumedad(900);
    TEST_ASSERT_EQUAL(100, estacion.getHumedad());
}

void testSetRadiacion() {
    // Test valores negativos y menores a 121
    estacion.setRadiacion(0);
    TEST_ASSERT_EQUAL(0, estacion.getRadiacion());
    estacion.setRadiacion(-10);
    TEST_ASSERT_EQUAL(0, estacion.getRadiacion());
    estacion.setRadiacion(-20);
    TEST_ASSERT_EQUAL(0, estacion.getRadiacion());
    estacion.setRadiacion(69);
    TEST_ASSERT_EQUAL(0, estacion.getRadiacion());
    estacion.setRadiacion(120);
    TEST_ASSERT_EQUAL(0, estacion.getRadiacion());
    estacion.setRadiacion(121);
    TEST_ASSERT_EQUAL(0, estacion.getRadiacion());

    // // Test valores muy altos
    estacion.setRadiacion(122);
    TEST_ASSERT_EQUAL(2, estacion.getRadiacion());
    estacion.setRadiacion(184);
    TEST_ASSERT_EQUAL(138, estacion.getRadiacion());
    estacion.setRadiacion(430);
    TEST_ASSERT_EQUAL(679, estacion.getRadiacion());
    estacion.setRadiacion(550);
    TEST_ASSERT_EQUAL(942, estacion.getRadiacion());
    estacion.setRadiacion(1024);
    TEST_ASSERT_EQUAL(1984, estacion.getRadiacion());

}

void testSetHoja() {
    /// Test valor de sensor 500 = Hoja mojada
    String stringMojada = "Mojada";
    estacion.setHoja(500);
    String valorMojada = estacion.getHumHoja();
    TEST_ASSERT_EQUAL_STRING(stringMojada.c_str(), valorMojada.c_str());

    /// Test valor de sensor 400 = Hoja muy humeda
    String stringMuyHumeda = "Muy humeda";
    estacion.setHoja(400);
    String valorMuyHumeda = estacion.getHumHoja();
    TEST_ASSERT_EQUAL_STRING(stringMuyHumeda.c_str(), valorMuyHumeda.c_str());

    /// Test valor de sensor 100 = Hoja seca
    String stringSeca = "Seca";
    estacion.setHoja(100);
    String valorSeca = estacion.getHumHoja();
    TEST_ASSERT_EQUAL_STRING(stringSeca.c_str(), valorSeca.c_str());

    
    /// Test valor de sensor 0 = Hoja seca
    String stringSecaCero = "Seca";
    estacion.setHoja(0);
    String valorSecaCero = estacion.getHumHoja();
    TEST_ASSERT_EQUAL_STRING(stringSecaCero.c_str(), valorSecaCero.c_str());

}


// TODO: Realizar los test cases correspondientes
void testSetDireccion() {
    //TEST_ASSERT_EQUAL();
}

int main(){
    UNITY_BEGIN();
    //RUN_TEST(test_loop);
    RUN_TEST(testSetHoja);
    RUN_TEST(testSetHumedad);
    RUN_TEST(testSetRadiacion);
    UNITY_END();
}
#endif
