// Copyright (c) 2020 aNoken

#include <M5Stack.h>
#include <WiFi.h>
/* ESP8266Audioライブラリの関数を読み込む */
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"

AudioGeneratorWAV *wav;   //wavファイルを扱うクラス
AudioFileSourceSD *file;  //SDカードからのファイルを扱うクラス
AudioOutputI2S *out;      //I2S出力を扱うクラス

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  WiFi.mode(WIFI_OFF);              //Wi-FiをOFF
  vTaskDelay(500);

  M5.Lcd.clear(BLACK);              //背景を白でクリアする
  M5.Lcd.setTextFont(5);            //文字の大きさを指定
  M5.Lcd.drawString("WAV Play...\n", 10, 50);  //文字を表示

  file = new AudioFileSourceSD("/sample.wav");// wavファイルを読み込む
  out = new AudioOutputI2S(0, 1);         //I2S出力を選択
  out->SetOutputModeMono(true);           //モノラルを選択
  wav = new AudioGeneratorWAV();          //wavファイルを生成
  wav->begin(file, out);                  //wavファイルを再生
}

void loop(){
  
  if (wav->isRunning()) {          //wavファイルが再生中の処理
    if (!wav->loop()) wav->stop();  //wavファイルが終了したら再生停止
  }
  else {                          //wavファイルが終了した後の処理
    M5.Lcd.drawString("WAV done\n", 10, 150);
      vTaskDelay(1000);
  }
}
