#include "LSM6DSLSensor.h"
DevI2C *i2c;
LSM6DSLSensor *sensor;
int32_t axes[3];
int16_t raws[3];
float data;
void setup(){
    i2c = new DevI2C(D14, D15);
    sensor = new LSM6DSLSensor(*i2c, D4, D5);
    // init
    sensor->init(NULL);
}
void loop(){
    // Accelerometer test
    accelerometer_test();
    // Gyroscope test
    gyroscope_test();
    delay(1000);
}
void accelerometer_test(){
    Serial.println("***Accelerator***");
    // enableAccelerator
    sensor->enableAccelerator();
    // getXAxes
    sensor->getXAxes(axes);
    Serial.printf("Axes: x: %d, y: %d, z: %d\n", axes[0], axes[1], axes[2]);
    // getXSensitivity
    sensor->getXSensitivity(&data);
    Serial.print("Sensitivity: ");
    Serial.println(data);
    // getXAxesRaw
    sensor->getXAxesRaw(raws);
    Serial.printf("Raw: x: %d, y: %d, z: %d\n", raws[0], raws[1], raws[2]);
}
void gyroscope_test(){
    Serial.println("***Gyroscope***");
    // enableGyroscope
    sensor->enableGyroscope();
    // getGAxes
    sensor->getGAxes(axes);
    Serial.printf("Axes: x: %d, y: %d, z: %d\n", axes[0], axes[1], axes[2]);
    // getGSensitivity
    sensor->getGSensitivity(&data);
    Serial.print("Sensitivity: ");
    Serial.println(data);
    // getGAxesRaw
    sensor->getGAxesRaw(raws);
    Serial.printf("Raw: x: %d, y: %d, z: %d\n", raws[0], raws[1], raws[2]);
}
