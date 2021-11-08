// Copyright (c) 2020 aNoken

#include <M5StickC.h>
const int motorL_adr = 0x60;
const int motorR_adr = 0x64;
long Speed;
long SpeedL, SpeedR;

/* I2Cモータードライバ・モジュール DRV8830 でモータを回転する*/
void motor_drive_i2c_control(int motor_adr, int speed, byte data1) {
  byte regValue = 0x80;
  regValue = abs(speed);
  if (regValue > 100) regValue = 100; //入力を上限100で制限
  regValue = regValue << data1;
  if (speed < 0) regValue |= 0x01;  	//逆方向に回転する
  else           regValue |= 0x02;  	//正方向に回転する

  Wire.beginTransmission(motor_adr);    //I2Cの通信開始
  Wire.write(0x00);
  Wire.write(regValue);
  Wire.endTransmission(true);           //I2Cの通信終了
}
void setup() {
  M5.begin();                           //M5StickCを初期化
  M5.Lcd.setRotation(3);                //画面の向きを指定
  M5.Lcd.setCursor(0, 30, 4);           //文字の先頭位置を指定
  M5.Lcd.println("glass porter");
  Wire.begin(32, 33, 10000); 		        //I2Cの通信開始
  SpeedL = 0;
  SpeedR = 0;
  motor_drive_i2c_control(motorL_adr, (SpeedL), 0x02);  //左側のモータを停止
  motor_drive_i2c_control(motorR_adr, (SpeedR), 0x02);  //右側のモータを停止
}

void loop() {
  M5.update();                        //M5StickCの内部状態を更新

  if (M5.BtnA.wasPressed()) {	          //Aボタンを押された時、その場で船体
    SpeedL = 100;
    SpeedR = -100;
    motor_drive_i2c_control(motorL_adr, (SpeedL), 0x02);
    motor_drive_i2c_control(motorR_adr, (SpeedR), 0x02);
  }
  if (M5.BtnB.wasPressed()) {	          //Bボタンを押された時、停止
    SpeedL = 0;
    SpeedR = 0;
    motor_drive_i2c_control(motorL_adr, (SpeedL), 0x02);
    motor_drive_i2c_control(motorR_adr, (SpeedR), 0x02);
  }
}
