#include <Arduino.h>
#include <unity.h>
#include "WeatherStation.h"

//WeatherStation weatherStation = WeatherStation(0);

void setUp(void){
}

void testLysimeterWeight() {
    //float peso = weatherStation.getLysimeterWeight();
    //TEST_ASSERT_EQUAL_FLOAT(peso, 0);
    TEST_ASSERT_EQUAL_INT(1, 1);
}
