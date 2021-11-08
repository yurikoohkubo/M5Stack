// Copyright (c) 2020 aNoken

#include <M5Stack.h> 
#include <WiFi.h> 
#include <WiFiClient.h> 
#include <WiFiAP.h> 

//Wi-FiのSSIDとパスワード
const char *ssid = "your_ssid"; 
const char *password = "your_passwd"; 
 
void setup_wifi_sta() { 
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化
  M5.Lcd.setTextFont(4); 
  M5.Lcd.println(); 
  M5.Lcd.print("Connecting to "); 
  M5.Lcd.println(ssid); 
  WiFi.begin(ssid, password);       //Wi-Fiと接続を開始

  while (WiFi.status() != WL_CONNECTED) { //接続完了するまで繰り返す
    vTaskDelay(500);
    M5.Lcd.print("."); 
  } 
  M5.Lcd.println(""); 
  M5.Lcd.println("WiFi connected"); 
  M5.Lcd.println("IP address: "); 
  M5.Lcd.println(WiFi.localIP());     //IPアドレスを表示する
} 
void setup() { 
  setup_wifi_sta(); 
} 
 
void loop() { 
} 
