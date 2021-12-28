// Copyright (c) 2020 aNoken

#include <M5Stack.h>

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  M5.Lcd.clear(BLACK);              //背景を黒でクリアする
  M5.Lcd.setTextColor(YELLOW);      //テキストの文字色を指定
  M5.Lcd.setTextSize(3);            //文字の大きさを指定
  M5.Lcd.setCursor(0, 0);            //文字の先端位置を指定
  M5.Lcd.println("Button example\n");//文字を表示
}

void loop() {
   M5.update();    					//M5Stackの内部処理を更新

  if (M5.BtnA.wasReleased())  {         //Aボタンを離した場合の処理
    M5.Lcd.println("A Release");        //文字を表示
  }
  else if (M5.BtnA.wasPressed())  {     //Aボタンを押した場合の処理
    M5.Lcd.println("Hello World!");          //文字を表示
  }
 
  else if (M5.BtnA.wasReleasefor(1000)) {
                           //Aボタンを離した後1000msec経過後の処理
    M5.Lcd.println("A wasReleasefor");    //文字を表示
  }

  else if (M5.BtnB.wasReleased())  {    //Bボタンを離した場合の処理
    M5.Lcd.println("B Release");        //文字を表示
  }
  else if (M5.BtnB.wasPressed())  {     //Bボタンを押した場合の処理
    M5.Lcd.println("B press");          //文字を表示
  }
  else if (M5.BtnB.wasReleasefor(1000)) {
                          //Bボタンを離した後1000msec経過後の処理
    M5.Lcd.println("B wasReleasefor");
  }
  if (M5.BtnC.wasPressed()){            //Cボタンを押した場合の処理
    M5.Lcd.clear(BLACK);            //背景を黒でクリアする
    M5.Lcd.setCursor(0, 0);         //文字の先端位置を指定
  }
}
