// Copyright (c) 2020 aNoken

#include <M5Stack.h> 
uint16_t counter;         //カウントアップする変数
 
void setup() { 
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  M5.Lcd.clear(WHITE);              //背景を白でクリアする
  M5.Lcd.setTextColor(BLUE, WHITE);   //テキストの文字色を指定
  counter = 0;              
} 

void loop() { 
   M5.update();    //M5Stackの内部処理を更新
 
  M5.Lcd.setCursor(20, 30);         //文字の先端位置を指定 
  M5.Lcd.setTextSize(3);            //文字の大きさを指定
  M5.Lcd.println("M5Stack Draw Num"); //文字を表示
  
  M5.Lcd.setCursor(120, 120);        //文字の先端位置を指定 
  M5.Lcd.setTextSize(6);           //文字の大きさを指定
  M5.Lcd.println(counter++);       //変数を表示
  vTaskDelay(10);
} 
