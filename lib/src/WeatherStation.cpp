#include "WeatherStation.h"

#define TIME_THRESHOLD 150
#define CALIBRATION 468.6

WeatherStation::WeatherStation(long initialCounter) {
    this -> initTime = 0;
    this -> pluviometerCounter = initialCounter;
}

WeatherStation::WeatherStation() {
    this -> initTime = 0;
    this -> pluviometerCounter = 0;
}
void WeatherStation::init() {
    this -> lysimeter.begin(16, 4);
    this -> lysimeter.set_scale(CALIBRATION);
    this -> lysimeter.tare();
    this -> bmp180Sensor.begin();
}

void WeatherStation::setWindSpeed(long int windSpeedSensor) {
    long int vel=0;
    if ( windSpeedSensor >= 6) {
        vel = ((windSpeedSensor - 6) * 240) / 2972;
    }
    this -> windSpeed = vel;
}

void WeatherStation::setwindDirection(int windDirectionSensor) {
    
    float valorVoltaje = fmap(windDirectionSensor, 0, 4095, 0.0, 3.3);
    this -> windDirection = (int) (valorVoltaje * 100);
}

float WeatherStation::fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void WeatherStation::setHumidity(int humiditySensorPort) {
    delay(2000); //Para respetar la frecuencia del sensor
    DHT sensorTH (humiditySensorPort, DHT22);
    this -> humidity = isnan(sensorTH.readHumidity()) ? 0 : sensorTH.readHumidity();
}

void WeatherStation::setRadiation(long int radiationSensor) {
    unsigned long int rad = 0;
    if (radiationSensor >= 10) {
        rad = ((radiationSensor - 10) * 1400) / 2596;
    }
    this -> radiation = rad > 1400 ? 1400 : rad;
}

void WeatherStation::setTemperature() {
    char status;
    double temperature;
    status = this -> bmp180Sensor.startTemperature();
    if (status != 0) {
        delay(status); 
        status = this -> bmp180Sensor.getTemperature(temperature); 
    }
    this -> temperature = static_cast<long int>(temperature);
}


void WeatherStation::setPresion() {
    double pressure;
    setTemperature();
    double temperature = this -> getTemperature(); 
    char status;
    status = bmp180Sensor.startPressure(3); 
    if (status != 0){        
        delay(status);
        status = bmp180Sensor.getPressure(pressure,temperature); 
    }      
    this -> pressure = static_cast<long int>(pressure); 
}


void WeatherStation::setLeafMoisture(int leafHumididtySensor) {
    this -> leafMoisture = round((leafHumididtySensor*100)/2668);
}

void WeatherStation::setPulseCounter(long int currentCount) {
    this -> pluviometerCounter += currentCount;
}

void WeatherStation::resetPulseCounter() {
    this -> pluviometerCounter = 0;
}

void WeatherStation::plantIrrigation(float ETc, float rainfall) {
    int timeout = 5000; // Timeout set to 5 seconds
    float DryWeightToday = getLysimeterWeight();
    float waterDensity = 1.0; // Water density in g/cm³

    // Check if the weight sensor is ready
    Serial.print("Lysimeter ready: ");
    Serial.println(lysimeter.is_ready() ? "Yes" : "No");   //debug
   // if (!lysimeter.is_ready()) {
   //     Serial.println("\nError: Unable to read the weight sensor. Irrigation will not proceed.");
   //     return; // Stop the function if the weight sensor is not ready
   // }

    float waterNeeded = ETc - rainfall;
    if (waterNeeded <= 0) {
        Serial.println("\nNo need to irrigate, rainfall is sufficient.");
    } else {
        float volume = (waterNeeded/10) * 1225;        // Convert mm to cm³ (Assuming 1225 cm2 lysimeter area)
        float RequiredIrrigation = volume * waterDensity;    // weight in grams
        Serial.println("\nThe amount of water to irrigate in grams is: ");
        Serial.println(RequiredIrrigation);                   // Verification of the weight to be added

        Serial.println("\nOpening irrigation control...");
        digitalWrite(IRRIGATION_CONTROL_PORT, HIGH);

        // Irrigate until the target weight is reached or timeout
        unsigned long startTime = millis();
        do {
            delay(50); // Wait 0.05 second between weight readings
            // Check if weight is increasing (indicating successful irrigation)
            if (getLysimeterWeight() <= DryWeightToday && (millis() - startTime) > timeout) {
                Serial.println("\nError: Weight is not increasing. Valve may be faulty or not working.");
                break;
            }
        } while (getLysimeterWeight() < DryWeightToday + RequiredIrrigation);

        Serial.println("\nClosing irrigation control...");
        digitalWrite(IRRIGATION_CONTROL_PORT, LOW);
    }
}


long int WeatherStation::getWindSpeed() {
    return this -> windSpeed;
}

int WeatherStation::getWindDirection() {
    return this -> windDirection;
}

float WeatherStation::getHumidity() {
    return this -> humidity;
}

unsigned long int WeatherStation::getRadiation() {
    return this -> radiation;
}

long int WeatherStation::getTemperature() {
    return this -> temperature;
}

long int WeatherStation::getPressure() {
    return this -> pressure;
}

int WeatherStation::getLeafMoisture() {
    return this -> leafMoisture;
}

float WeatherStation::getPluviometerCounter() {
    return this -> pluviometerCounter*0.25;
}

float WeatherStation::getLysimeterWeight() {
    return this -> lysimeter.get_units(4);
}


String WeatherStation::getPayload() {
    String jsonPayload = "{";
    jsonPayload += "\"pluviometer\":" + String(this ->  getPluviometerCounter());
    jsonPayload += ",\"windspeed\":" + String(this ->  getWindSpeed());
    jsonPayload += ",\"winddirection\":" + String(this ->  getWindDirection());
    jsonPayload += ",\"leafmoisture\":" + String(this ->  getLeafMoisture());
    jsonPayload += ",\"humidity\":" + String(this ->  getHumidity());
    jsonPayload += ",\"radiation\":" + String(this ->  getRadiation());
    jsonPayload += ",\"temperature\":" + String(this ->  getTemperature());
    jsonPayload += ",\"pressure\":" + String(this ->  getPressure());
    jsonPayload += ",\"weight\":" + String(this ->  getLysimeterWeight(), 4);
    jsonPayload += "}";

    this -> resetPulseCounter();

    return jsonPayload;
}