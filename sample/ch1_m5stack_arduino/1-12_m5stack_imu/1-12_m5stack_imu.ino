// Copyright (c) 2020 aNoken

#define M5STACK_MPU6886
// #define M5STACK_MPU9250
// #define M5STACK_MPU6050
// #define M5STACK_200Q

#include <M5Stack.h>

float accX = 0.0, accY = 0.0, accZ = 0.0;

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  M5.IMU.Init();                    //IMUを初期化
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
}

void loop() {
  M5.IMU.getAccelData(&accX, &accY, &accZ); //IMUから加速度を取得
  M5.Lcd.setCursor(0, 65);
  M5.Lcd.printf("X %5.2f Y %5.2f Z %5.2f   ", accX, accY, accZ);
  M5.Lcd.setCursor(220, 87);
  M5.Lcd.print(" G");
  vTaskDelay(10);
}
