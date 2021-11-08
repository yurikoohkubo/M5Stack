// Copyright (c) 2020 aNoken 
// Arduino IDE compile code

#include <M5Stack.h>
HardwareSerial serial_ext(2);

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  serial_ext.begin(115200, SERIAL_8N1, 17, 16);//UARTの初期化
}

void loop() {
  M5.update();   //M5Stackの内部処理を更新
  
  const int thresh = 1000;

 	//文字データを受信して、数価データへ変換する
  if ( serial_ext.available() > 0 ) {
    String str = serial_ext.readStringUntil('\n');
    int data = str.toInt();
    Serial.print("data:");
    Serial.println(data);
    if (data > thresh) {
      Serial.print("異常が発生しました");
    }
  }

  vTaskDelay(10 / portTICK_RATE_MS);
}


