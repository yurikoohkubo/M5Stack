// Copyright (c) 2020 aNoken

#include <M5Stack.h>

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  M5.Lcd.clear(BLACK);              //背景を黒でクリアする
}
void loop() {
   M5.update();    //M5Stackの内部処理を更新
  
  int width = M5.Lcd.width() - 1;   //LCDの幅を取得
  int height = M5.Lcd.height() - 1; //LCDの高さを取得

  /* 三角形をランダムに表示する*/
  M5.Lcd.fillTriangle(random(width), random(height), random(width),
    random(height), random(width), random(height), random(0xfffe));
}
