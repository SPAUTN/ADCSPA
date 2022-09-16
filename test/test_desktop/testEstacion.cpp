#include "estacion.h"
#include "unity.h"

#ifdef UNIT_TEST


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
    UNITY_END();
}

#endif
