// Copyright (c) 2020 aNoken

//以下のURL（AquesTalkの作者のHP）から「SampleTTS.zip」をダウンロ
//ードし、「AquesTalkTTS.cpp」と「AquesTalkTTS.h」を入手しましょう。
//http://blog-yama.a-quest.com/?eid=970195

#include <M5Stack.h>
#include "AquesTalkTTS.h"
const float V_REF = 3.00;

/* GROVE-I2C ADCを初期化する */
void init_adc() {
  Wire.begin();
  Wire.beginTransmission(0x50);
  Wire.write(0x02);
  Wire.write(0x20);
  Wire.endTransmission();
}
/* GROVE-I2C ADCから圧力センサーのアナログ値を読み出す */
float read_adc() {
  unsigned int getData;
  static float analogVal = 0;
  Wire.beginTransmission(0x50);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x50, 2);
  delay(1);
  if (Wire.available() <= 2)  {
    getData = (Wire.read() & 0x0f) << 8;
    getData |= Wire.read();
  }
  delay(5);
  //アナログの読み取り値を移動平均してノイズを除去。
  analogVal = 0.9 * analogVal + 0.1 * getData * V_REF * 2.0 / 4096.0;
  return analogVal;
}
void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  init_adc();
}
void loop() {
  const float thresh_max = 3.5;	//プリンの閾値_最大
  const float thresh_min = 3.0;	//プリンの閾値_最小
  const float thresh_none = 1.0;	//なにも置いていないときの重さ
  float value = read_adc();

  /*プリンの重さが許容範囲内であればなにもしない*/
  if ((value < thresh_max) || (value >= thresh_min)) {
    vTaskDelay(10);
  }
  /* プリンの重さが違う時は「プリンじゃないよ」としゃべる */
  else if (value > thresh_none) {
    TTS.play("purin,ja,naiyo", 100);
    vTaskDelay(1000);
  }
  /* 何も置いてない時は「プリンがとられた」としゃべる */
  else {
    TTS.play("purin,ga,torareta", 100);
    vTaskDelay(1000);
  }

  vTaskDelay(50);
}
