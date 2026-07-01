#include <Arduino.h> 
#include <Wire.h>   // I2C 
#include <DFRobot_BMI160.h>   //BMI160

DFRobot_BMI160 bmi160;    // stworzenie obiektu bmi160 (6-axis gyro)

// fizyczne połączenie ESP32 <-> BMI160
const int SDA_PIN = 8;  
const int SCL_PIN = 9;

// tablice odczytow z akcelerometru i zyroskopu [0] -> X | [1] -> Y | [2] -> Z
int16_t acc[3], gyro[3];

// tablice i zmienne pomocnicze do usuniecia offsetu akcelerometru i zyroskopu
int16_t acc_offset[3], gyro_offset[3]; 
long acc_offset_x, acc_offset_y, acc_offset_z, gyro_offset_x, gyro_offset_y, gyro_offset_z;

int calibration_samples = 200;

void setup() {
  Serial.begin(115200);   // prędkość komunikacji 115200 bitow na sekunde
  delay(1000);  // (ms) esp32 potrzebuje czasu, wczesniej wpadał w bootloopa 

  Wire.begin(SDA_PIN, SCL_PIN);    

  //obsluga bledu I2C, gdy wszystko dziala pod adresem 0x69 ma byc zerO!
  if (bmi160.I2cInit(0x69) != 0) {
    Serial.println("BMI160 FAIL");
    while (1);
  }

  Serial.println("BMI160 OK");
  Serial.println("BMI160 Calibrating");
  for (int i = 0; i < calibration_samples; ++i){
    bmi160.getAccelData(acc_offset);
    bmi160.getGyroData(gyro_offset);
    acc_offset_x += acc_offset[0];
    acc_offset_y += acc_offset[1];
    acc_offset_z += acc_offset[2];
    gyro_offset_x += gyro_offset[0];
    gyro_offset_y += gyro_offset[1];
    gyro_offset_z += gyro_offset[2];
    delay(10);
  }
  acc_offset_x /= calibration_samples;
  acc_offset_y /= calibration_samples;
  acc_offset_z /= calibration_samples;
  gyro_offset_x /= calibration_samples;
  gyro_offset_y /= calibration_samples;
  gyro_offset_z /= calibration_samples;
  Serial.println("Done calibrating :)");
}

void loop() {
  // funkcje z biblioteki do BMI160. Odczyt danych i kopia do tablic "acc" i "gyro"
  bmi160.getAccelData(acc);
  bmi160.getGyroData(gyro);

  // wyswietlanie danych
  Serial.print("ACC: ");
  Serial.print(acc[0] - acc_offset_x); Serial.print(" ");
  Serial.print(acc[1] - acc_offset_y); Serial.print(" ");
  Serial.println(acc[2] - acc_offset_z);

  Serial.print("GYRO: ");
  Serial.print(gyro[0] - gyro_offset_x); Serial.print(" ");
  Serial.print(gyro[1] - gyro_offset_y); Serial.print(" ");
  Serial.println(gyro[2] - gyro_offset_z);

  delay(100); 
}