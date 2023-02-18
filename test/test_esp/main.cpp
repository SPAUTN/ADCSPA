#include <Arduino.h>
#include <unity.h>

#include "WeatherStation.h"

#include "sensorsWithExternalLibs.h"
#include "sensorsWithoutExternalLibs.h"

void setup() {
    UNITY_BEGIN();
    RUN_TEST(testsetLeafMoisture);
    RUN_TEST(testsetHumidity);
    RUN_TEST(testLysimeterWeight);
    UNITY_END();
}

void loop() {}