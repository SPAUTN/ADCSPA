#include <Arduino.h>
#include <unity.h>
#include "WeatherStation.h"

WeatherStation weatherStation = WeatherStation(0);

void setUp(void){
}


void testsetHumidity() {

    // Test valores negativos y menores a 102
    weatherStation.setHumidity(0);
    TEST_ASSERT_EQUAL(0, weatherStation.getHumidity());
    weatherStation.setHumidity(-10);
    TEST_ASSERT_EQUAL(0, weatherStation.getHumidity());
    weatherStation.setHumidity(-20);
    TEST_ASSERT_EQUAL(0, weatherStation.getHumidity());
    weatherStation.setHumidity(100);
    TEST_ASSERT_EQUAL(0, weatherStation.getHumidity());
    weatherStation.setHumidity(102);
    TEST_ASSERT_EQUAL(0, weatherStation.getHumidity());

    // Test valores menores a 372
    weatherStation.setHumidity(103);
    TEST_ASSERT_EQUAL(0, weatherStation.getHumidity());
    weatherStation.setHumidity(110);
    TEST_ASSERT_EQUAL(0 , weatherStation.getHumidity());
    weatherStation.setHumidity(120);
    TEST_ASSERT_EQUAL(0 , weatherStation.getHumidity());

    // Valores específicos de humedad
    // Humedad 25%
    /*weatherStation.setHumidity(281);
    TEST_ASSERT_EQUAL(0, weatherStation.getHumidity());
    // Humedad 40%
    weatherStation.setHumidity(389);
    TEST_ASSERT_EQUAL(40, weatherStation.getHumidity());
    // Humedad 45%
    weatherStation.setHumidity(425);
    TEST_ASSERT_EQUAL(45, weatherStation.getHumidity());
        // Humedad 47%
        weatherStation.setHumidity(439);
    TEST_ASSERT_EQUAL(47, weatherStation.getHumidity());
    // Humedad de 50%
    weatherStation.setHumidity(460);
    TEST_ASSERT_EQUAL(50, weatherStation.getHumidity());
    // Humedad de 75%
    weatherStation.setHumidity(639);
    TEST_ASSERT_EQUAL(75, weatherStation.getHumidity());
    // Humedad de 100%
    weatherStation.setHumidity(818);
    TEST_ASSERT_EQUAL(100, weatherStation.getHumidity());*/

    // Test valor mayor a 3152
    weatherStation.setHumidity(3153);
    TEST_ASSERT_EQUAL(100, weatherStation.getHumidity());
}

void testsetRadiation() {
    // Test valores negativos y menores a 121
    weatherStation.setRadiation(0);
    TEST_ASSERT_EQUAL(0, weatherStation.getRadiation());
    weatherStation.setRadiation(-10);
    TEST_ASSERT_EQUAL(0, weatherStation.getRadiation());
    weatherStation.setRadiation(-20);
    TEST_ASSERT_EQUAL(0, weatherStation.getRadiation());
    weatherStation.setRadiation(69);
    TEST_ASSERT_EQUAL(0, weatherStation.getRadiation());
    weatherStation.setRadiation(120);
    TEST_ASSERT_EQUAL(0, weatherStation.getRadiation());
    weatherStation.setRadiation(121);
    TEST_ASSERT_EQUAL(0, weatherStation.getRadiation());

    // // Test valores muy altos
    weatherStation.setRadiation(122);
    TEST_ASSERT_EQUAL(2, weatherStation.getRadiation());
    weatherStation.setRadiation(184);
    TEST_ASSERT_EQUAL(138, weatherStation.getRadiation());
    weatherStation.setRadiation(430);
    TEST_ASSERT_EQUAL(679, weatherStation.getRadiation());
    weatherStation.setRadiation(550);
    TEST_ASSERT_EQUAL(942, weatherStation.getRadiation());
    weatherStation.setRadiation(1024);
    TEST_ASSERT_EQUAL(1984, weatherStation.getRadiation());

}

void testsetLeafMoisture() {
    /// Test valor de sensor 500 = Hoja mojada
    int expected = 12;
    weatherStation.setLeafMoisture(500);
    TEST_ASSERT_EQUAL_INT(expected, weatherStation.getLeafMoisture());

    /// Test valor de sensor 400 = Hoja muy humeda
    expected = 9;
    weatherStation.setLeafMoisture(400);
    TEST_ASSERT_EQUAL_INT(expected, weatherStation.getLeafMoisture());

    // Test valor de sensor 100 = Hoja seca
    expected = 2;
    weatherStation.setLeafMoisture(100);
    TEST_ASSERT_EQUAL_INT(expected, weatherStation.getLeafMoisture());
    
    /// Test valor de sensor 0 = Hoja seca
    expected = 0;
    weatherStation.setLeafMoisture(0);
    TEST_ASSERT_EQUAL_INT(expected, weatherStation.getLeafMoisture());

}


// TODO: Realizar los test cases correspondientes
void testSetDireccion() {
    //TEST_ASSERT_EQUAL();
}
/*
void testLysimeterWeight() {
    float peso = weatherStation.getLysimeterWeight();
    TEST_ASSERT_EQUAL_FLOAT(peso, 0);
}
*/
void setup() {
    weatherStation.init();
    UNITY_BEGIN();
    RUN_TEST(testsetLeafMoisture);
    RUN_TEST(testsetHumidity);
    //RUN_TEST(testLysimeterWeight);
    UNITY_END();
}

void loop(){}

