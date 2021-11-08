// Copyright (c) 2020 aNoken

#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

//Wi-FiのSSIDとパスワード
const char *ssid = "M5Stack_AP";
const char *password = "your_passwd";

void setup_wifi_ap() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化

  M5.Lcd.setTextFont(4);

  WiFi.softAP(ssid, password);    //Wi-Fiのアクセスポイントを起動
  M5.Lcd.print("\n AP Name: ");
  M5.Lcd.println("ssid");
  IPAddress myIP = WiFi.softAPIP();
  M5.Lcd.println("\n AP IP address: "); //IPアドレスを表示する
  M5.Lcd.println(myIP);
}
void setup() {
  setup_wifi_ap();
}

void loop() {
}
