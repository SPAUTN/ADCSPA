#include "estacion.h"
#include "unity.h"

#ifdef UNIT_TEST

void testSetHumedad(){

    // Test valores negativos y menores a 102
    TEST_ASSERT_EQUAL(0, setHumedad(0));
    TEST_ASSERT_EQUAL(0, setHumedad(-10));
    TEST_ASSERT_EQUAL(0, setHumedad(-20));
    TEST_ASSERT_EQUAL(0, setHumedad(100));
    TEST_ASSERT_EQUAL(0, setHumedad(102));

    // Test valores mayores a 102
    TEST_ASSERT_EQUAL( 0.14, setHumedad(103));
    TEST_ASSERT_EQUAL( ((110 - 102) * 5 * 20) / 716, setHumedad(110));
    TEST_ASSERT_EQUAL( ((120 - 102) * 5 * 20) / 716, setHumedad(120));

    // Valores específicos de humedad
    // Humedad 25%
    TEST_ASSERT_EQUAL(25, setHumedad(281));
    // Humedad 40%
    TEST_ASSERT_EQUAL(40, setHumedad(389));
    // Humedad 45%
    TEST_ASSERT_EQUAL(45, setHumedad(425));
        // Humedad 47%
    TEST_ASSERT_EQUAL(47, setHumedad(439));
    // Humedad de 50%
    TEST_ASSERT_EQUAL(50, setHumedad(460));
    // Humedad de 75%
    TEST_ASSERT_EQUAL(75, setHumedad(639));
    // Humedad de 100%
    TEST_ASSERT_EQUAL(100, setHumedad(818));

    // Test valor mayor a 818
    TEST_ASSERT_EQUAL(100, setHumedad(900));
    

}

void testSetRadiacion(){
    // Test valores negativos y menores a 41
    TEST_ASSERT_EQUAL(0, setRadiacion(0));
    TEST_ASSERT_EQUAL(0, setRadiacion(-10));
    TEST_ASSERT_EQUAL(0, setRadiacion(-20));

    // Test valores muy altos
    TEST_ASSERT_EQUAL(760, setRadiacion(430));
    TEST_ASSERT_EQUAL(995, setRadiacion(550));
    TEST_ASSERT_EQUAL(1922, setRadiacion(1024));

}

void testSetHoja(){

    /// Test valor de sensor 500 = Hoja mojada
    String stringMojada = "Mojada";
    String valorMojada = setHoja(500);
    TEST_ASSERT_EQUAL_STRING(stringMojada.c_str(), valorMojada.c_str());

    /// Test valor de sensor 400 = Hoja muy humeda
    String stringMuyHumeda = "Muy humeda";
    String valorMuyHumeda = setHoja(400);
    TEST_ASSERT_EQUAL_STRING(stringMuyHumeda.c_str(), valorMuyHumeda.c_str());

    /// Test valor de sensor 100 = Hoja seca
    String stringSeca = "Seca";
    String valorSeca = setHoja(100);
    TEST_ASSERT_EQUAL_STRING(stringSeca.c_str(), valorSeca.c_str());

    
    /// Test valor de sensor 0 = Hoja seca
    String stringSecaCero = "Seca";
    String valorSecaCero = setHoja(0);
    TEST_ASSERT_EQUAL_STRING(stringSecaCero.c_str(), valorSecaCero.c_str());

}

int main(){
    UNITY_BEGIN();
    RUN_TEST(testSetHoja);
    RUN_TEST(testSetHumedad);
    RUN_TEST(testSetRadiacion);
    UNITY_END();
}

#endif
