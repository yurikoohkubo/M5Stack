// Copyright (c) 2020 aNoken

#include <ssl_client.h>
#include <WiFiClientSecure.h>
#include <M5Stack.h>

/* Wi-Fi環境設定 */
const char* ssid = "your_ssid";
const char* password = "your_passwd";

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化

  wifi_connect();   //Wi-Fiに接続する
  send_line_alert();  //LINEへの送信
}
void loop() {
  M5.update();    //M5Stackの内部処理を更新
}
/* Wi-Fiに接続する */
void wifi_connect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)  vTaskDelay(500);
  Serial.println(WiFi.localIP());
}
/* LINEへの送信 */
void send_line_alert() {
  /* LINENofifyの設定 */
  const char* host = "notify-api.line.me";
  const char* token = "your_token";

  const char* message1 = "プリンが取られました！！！";
  WiFiClientSecure client;
  if (!client.connect(host, 443)) {
    return;
  }
  /* LINE NotifyのサーバへHTTPS通信でメッセージをPOSTする */
  String query = "message=" + String(message1);
  String request = "POST /api/notify HTTP/1.1\r\n";
  request += "Host:" + String(host) + "\r\n";
  request += "Authorization:Bearer " + String(token) + "\r\n";
  request += "Content-Length:" + String(query.length()) + "\r\n";
  request += "Content-Type: application/x-www-form-urlencoded\r\n\r\n" + String(query) + "\r\n";
  client.print(request);
  Serial.print(request);

  while (client.connected()) {
    String line_str = client.readStringUntil('\n');
    if (line_str == "\r")   break;
  }
}
