#include "HTS221Sensor.h"
DevI2C *i2c;
HTS221Sensor *sensor;
float humidity = 0;
float temperature = 0;
unsigned char id;
void setup() {
    i2c = new DevI2C(D14, D15);
    sensor = new HTS221Sensor(*i2c);
    // init the sensor
    sensor -> init(NULL);
}
void loop() {
    // enable
    sensor -> enable();
    // read id
    sensor -> readId(&id);
    Serial.printf("ID: %d\r\n", id);
    // get humidity
    sensor -> getHumidity(&humidity);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    // get temperature
    sensor -> getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    // disable the sensor
    sensor -> disable();
    // reset
    sensor -> reset();
    delay(1000);
}
