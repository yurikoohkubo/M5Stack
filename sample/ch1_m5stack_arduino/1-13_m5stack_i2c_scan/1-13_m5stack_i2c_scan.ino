// Copyright (c) 2020 aNoken

#include <M5Stack.h>
int textColor = YELLOW;

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  
  Wire.begin();                     //I2C通信を開始
  vTaskDelay(1000);
}
void loop() {
  int address;
  int error;
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("I2C scanning Address [HEX]");

  /* I2Cのアドレス一覧を表示　*/
  for (address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);   //I2Cへ通信開始
    error = Wire.endTransmission();    //I2Cの通信終了
    if (error == 0) {                  //レスポンスあるアドレスを表示
      M5.Lcd.print("0x");
      M5.Lcd.print(address, HEX);
      M5.Lcd.print(" ");
    }
    else M5.Lcd.print(".");
    vTaskDelay(10);
  }

  if (textColor == YELLOW) textColor = GREEN;
  else textColor = YELLOW;
  M5.Lcd.setTextColor(textColor, BLACK);
}
