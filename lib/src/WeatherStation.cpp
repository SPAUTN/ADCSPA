#include "WeatherStation.hpp"

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
    this -> dht = new DHT(HUMIDITY_SENSOR_PORT, DHT22);
    this -> dht -> begin();
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

void WeatherStation::setHumidity() {
    int attempts = 0;
    delay(2000);
    do {
        this -> humidity = this->dht->readHumidity();
        attempts++;
        delay(2000);
    } while (isnan(this -> humidity) && attempts <= ATTEMPTS);

    if (isnan(this -> humidity)) {
        throw std::runtime_error("ERROR: Unable to obtain a valid humidity reading after several attempts.");
    }
}

void WeatherStation::setRadiation(long int radiationSensor) {
    unsigned long int rad = 0;
    if (radiationSensor >= 270) {
        rad = ((radiationSensor - 270) * 1400) / 3651;
    }
    this -> radiation = rad > 1400 ? 1400 : rad;
}

void WeatherStation::setTemperature() {
    char status;
    double temperature;
    int attempts = 0;
    do {
        status = this -> bmp180Sensor.startTemperature();
        attempts++;
    } while(status != 0 && attempts <= ATTEMPTS);

    if (status != 0) {
        delay(status); 
        status = this -> bmp180Sensor.getTemperature(temperature); 
    } else {
        throw std::runtime_error("ERROR: Unable to obtain a valid temperature reading.");
    }
    this -> temperature = static_cast<long int>(temperature);
}


void WeatherStation::setPresion() {
    char status;
    double pressure;
    setTemperature();
    double temperature = this -> getTemperature(); 
    int attempts = 0;
    do {
        status = bmp180Sensor.startPressure(3); 
        attempts++;
    } while(status != 0 && attempts <= ATTEMPTS);
    
    if (status != 0){        
        delay(status);
        status = bmp180Sensor.getPressure(pressure,temperature); 
    } else {
        throw std::runtime_error("ERROR: Unable to obtain a valid pressure reading.");
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

float WeatherStation::irrigateAndGetETc(float wetweight) {
    // Check if the weight sensor is ready
    Serial.print("Lysimeter ready: ");
    Serial.println(!lysimeter.is_ready() ? "Yes" : "No");   //debug
    if (lysimeter.is_ready()) {
        throw std::runtime_error("ERROR:Unable to read the weight sensor. Irrigation will not proceed.");
    } else {
        int timeout = 10000; // Timeout set to 10 seconds
        int lysimeterArea = 1225;  //cm2
        float currentDryWeight = this -> getLysimeterWeight();
        float waterDensity = 1.0; // Water density in g/cm³
        float currentETc = ((wetweight - currentDryWeight)/lysimeterArea) * 10;
        if (currentETc <= 0) {
            Serial.println("\nNo need to irrigate, rainfall is sufficient.");
        } else {
            float volume = (currentETc / 10) * lysimeterArea;                   // Convert mm to cm³ 
            float RequiredIrrigation = volume * waterDensity;                   // weight in grams
            Serial.println("\nThe amount of water to irrigate in grams is: ");
            Serial.println(RequiredIrrigation);                                 // Verification of the weight to be added

            Serial.println("\nOpening irrigation control...");
            digitalWrite(IRRIGATION_CONTROL_PORT, HIGH);

            // Irrigate until the target weight is reached or timeout
            unsigned long startTime = millis();
            do {
                delay(50); // Wait 0.05 second between weight readings
                // Check if weight is increasing (indicating successful irrigation)
                if (getLysimeterWeight() <= currentDryWeight && (millis() - startTime) > timeout) {
                    Serial.println("\nError: Weight is not increasing. Valve may be faulty or not working.");
                    break;
                }
            } while (getLysimeterWeight() < currentDryWeight + RequiredIrrigation);

            Serial.println("\nClosing irrigation control...");
            digitalWrite(IRRIGATION_CONTROL_PORT, LOW);
        }
        return currentETc;
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
    String payload = "+pl:" + String(this ->  getPluviometerCounter());       // pluviometer
    payload += ";ws:" + String(this ->  getWindSpeed());                    // wind speed   
    payload += ";wd:" + String(this ->  getWindDirection());                // wind direction
    payload += ";l:" + String(this ->  getLeafMoisture());                  // leaf moisture
    payload += ";h:" + String(this ->  getHumidity());                      // humidity
    payload += ";r:" + String(this ->  getRadiation());                     // radiation
    payload += ";t:" + String(this ->  getTemperature());                   // temperature
    payload += ";pr:" + String(this ->  getPressure());                      // pressure
    payload += ";wh:" + String(this ->  getLysimeterWeight(), 4);            // weight    

    this -> resetPulseCounter();

    return payload;
}