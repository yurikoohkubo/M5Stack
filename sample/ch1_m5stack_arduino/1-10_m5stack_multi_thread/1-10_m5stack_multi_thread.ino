// Copyright (c) 2020 aNoken

#include <M5Stack.h>

/* task0のループ関数 */
void task0(void* arg) {
  static int cnt = 0;
  //カウントアップしてシリアルとLCDに表示する
  while (1) {
    Serial.printf("task0 thread_cnt=%ld\n", cnt);
    M5.Lcd.printf("task0 thread_cnt=%ld\n", cnt);
    cnt++;
    vTaskDelay(1000);
  }
}

/* task1のループ関数 */
void task1(void* arg) {
  static int cnt = 0;
  //カウントアップしてシリアルとLCDに表示する
  while (1) {
    Serial.printf("task1 thread_cnt=%ld\n", cnt);
    M5.Lcd.printf("task1 thread_cnt=%ld\n", cnt);
    cnt++;
    vTaskDelay(1500);
  }
}

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0, 0);

  //task0のループ関数を起動
  xTaskCreatePinnedToCore(task0, "Task0", 4096, NULL, 1, NULL, 1);
  
  //task1のループ関数を起動
  xTaskCreatePinnedToCore(task1, "Task1", 4096, NULL, 2, NULL, 1);
}

void loop() {  //Arduinoのメインのループ関数はここで実行
  static int cnt = 0;

  //カウントアップしてシリアルとLCDに表示する
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Maintask thread_cnt=%ld\n", cnt);
  Serial.printf("Maintask thread_cnt=%ld\n", cnt);
  cnt++;
  vTaskDelay(1200);
}
