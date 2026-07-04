#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_BMI160.h>
#include <NimBLEDevice.h>

DFRobot_BMI160 bmi160;
NimBLECharacteristic *pCharacteristic;

const int SDA_PIN = 8;
const int SCL_PIN = 9;

int16_t acc[3], gyro[3];
int16_t acc_offset[3], gyro_offset[3];

long acc_offset_x = 0, acc_offset_y = 0, acc_offset_z = 0;
long gyro_offset_x = 0, gyro_offset_y = 0, gyro_offset_z = 0;

const int calibration_samples = 300;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin(SDA_PIN, SCL_PIN);

    if (bmi160.I2cInit(0x69) != 0) {
        Serial.println("BMI160 FAIL");
        while (1);
    }

    Serial.println("BMI160 OK - calibrating");

    for (int i = 0; i < calibration_samples; i++) {
        bmi160.getAccelData(acc_offset);
        bmi160.getGyroData(gyro_offset);

        acc_offset_x += acc_offset[0];
        acc_offset_y += acc_offset[1];
        acc_offset_z += acc_offset[2];

        gyro_offset_x += gyro_offset[0];
        gyro_offset_y += gyro_offset[1];
        gyro_offset_z += gyro_offset[2];

        delay(5);
    }

    acc_offset_x /= calibration_samples;
    acc_offset_y /= calibration_samples;
    acc_offset_z /= calibration_samples;

    gyro_offset_x /= calibration_samples;
    gyro_offset_y /= calibration_samples;
    gyro_offset_z /= calibration_samples;

    NimBLEDevice::init("ESP32_BMI160");

    NimBLEServer *server = NimBLEDevice::createServer();

    NimBLEService *service =
        server->createService("12345678-1234-1234-1234-1234567890ab");

    pCharacteristic =
        service->createCharacteristic(
            "87654321-4321-4321-4321-ba0987654321",
            NIMBLE_PROPERTY::READ |
            NIMBLE_PROPERTY::NOTIFY
        );

    service->start();

    NimBLEAdvertising *adv = NimBLEDevice::getAdvertising();
    adv->addServiceUUID(service->getUUID());
    adv->setName("ESP32_BMI160");
    adv->enableScanResponse(true);

    NimBLEDevice::startAdvertising();

    Serial.println("BLE READY");
}

void loop() {
    bmi160.getAccelData(acc);
    bmi160.getGyroData(gyro);

    int16_t ax = acc[0] - acc_offset_x;
    int16_t ay = acc[1] - acc_offset_y;
    int16_t az = acc[2] - acc_offset_z;

    int16_t gx = gyro[0] - gyro_offset_x;
    int16_t gy = gyro[1] - gyro_offset_y;
    int16_t gz = gyro[2] - gyro_offset_z;

    char buffer[80];
    sprintf(buffer, "%d,%d,%d,%d,%d,%d",
            ax, ay, az, gx, gy, gz);

    pCharacteristic->setValue(buffer);
    pCharacteristic->notify();

    delay(50); // ~20 Hz na start
}