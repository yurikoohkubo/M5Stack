// Copyright (c) 2020 aNoken

#include <M5Stack.h>
void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  M5.Lcd.setTextSize(2);            //文字の大きさを指定
  M5.Lcd.println("M5Stack Speaker");  //文字を表示
  M5.Speaker.setBeep(3000, 200);    //ビープ音を設定
}

void loop() {
  M5.update();                      //M5Stackの内部状態を更新

  if (M5.BtnA.wasPressed()) {    //Aボタンを押された場合ビープ音を鳴らす
    M5.Lcd.printf("A wasPressed \r\n");
    M5.Speaker.beep(); //beep
  }
  if (M5.BtnB.wasPressed())  {   //Bボタンを押された場合のtone音を鳴らす
    M5.Lcd.printf("B wasPressed \r\n");
    M5.Speaker.tone(3000, 200);
  }

}
