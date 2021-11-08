// Copyright (c) 2020 aNoken
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

//Wi-FiのSSIDとパスワードを指定
const char* ssid = "your_ssid";
const char* passwd = "your_passwd";

//Webサーバのクラスを定義
WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  setup_wifi();                          //Wi-Fiと接続する
  server.on("/", handleRoot);           //ルートパスでの処理を指定
  server.onNotFound(handleNotFound);    //パスがない処理を指定
  server.begin();                       //Webサーバを起動
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();                //Webサーバを監視する
}

void handleRoot() {              //ルートパスでの処理を定義する関数
  server.send(200, "text/plain", "hello from M5Camera!");
}

void handleNotFound() {         //URLが見つからない場合の処理を定義する関数
  server.send(404, "text/plain", "404 Error");
}

void setup_wifi() {             //Wi-Fiとの接続処理を行う関数
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);     //Wi-Fi雪像を開始
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {  //接続まで待機
    vTaskDelay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
