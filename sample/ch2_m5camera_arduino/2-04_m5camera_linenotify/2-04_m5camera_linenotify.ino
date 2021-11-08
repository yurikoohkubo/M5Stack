// Copyright (c) 2020 aNoken

#include "esp_camera.h"
#include <WiFi.h>
#include <ssl_client.h>
#include <WiFiClientSecure.h>

//Wi-FiのSSIDとパスワードを指定
const char* ssid = "your_ssid";
const char* passwd = "your_passwd";

#define CAMERA_MODEL_M5STACK_WIDE
#include "camera_pins.h"

int pin_gpio = 13;

void setup() {
  Serial.begin(115200);
  Serial.println();
  setup_camera();             //Cameraを設定する
  setup_wifi();               //Wi-Fiを設定する
  vTaskDelay(1000);

  camera_capture_sendline();          //Cameraで撮影し、Lineへ送信
  pinMode(pin_gpio, INPUT);
}

void loop() {
  bool M5Stack_Val = digitalRead(pin_gpio);
  static bool M5Stack_Val_old = M5Stack_Val;

  if ((M5Stack_Val == true) && (M5Stack_Val_old == false)) {  //GPIOがHIGH->Lowで実行
    camera_capture_sendline();    //Cameraで撮影し、Lineへ送信
  }
  M5Stack_Val_old = M5Stack_Val;
  vTaskDelay(50);
}

void camera_capture_sendline() {//Cameraで撮影し、Lineへ送信する関数
  uint32_t data_len = 0;
  camera_fb_t * fb = esp_camera_fb_get();
  Serial.printf("image size%d[byte]:width%d,height%d,format%d\r\n",
                fb->len, fb->width, fb->height, fb->format);
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  sendLineNotify(fb->buf, fb->len);
  esp_camera_fb_return(fb);
  vTaskDelay(5000);
}

//Line Notify へ写真送信する
void sendLineNotify(uint8_t* image_data, size_t image_sz) {
  /* LINENofifyの設定 */
  const char* host = "notify-api.line.me";
  const char* token = "your_token";

  WiFiClientSecure client;
  if (!client.connect(host, 443))   return;
  int httpCode = 404;
  size_t image_size = image_sz;

  //マルチパートでのHTTP分割送信
  String boundary = "----purin_alert--";
  String body = "--" + boundary + "\r\n";
  String message = "プリンがとられた！！！";
  body += "Content-Disposition: form-data;name=\"message\"\r\n\r\n" + message + " \r\n";
  if (image_data != NULL && image_sz > 0 ) {
    image_size = image_sz;
    body += "--" + boundary + "\r\n";
    body += "Content-Disposition: form-data; name=\"imageFile\"; filename=\"image.jpg\"\r\n";
    body += "Content-Type: image/jpeg\r\n\r\n";
  }
  String body_end = "--" + boundary + "--\r\n";
  size_t body_length = body.length() + image_size + body_end.length();

  //マルチパートのHTTPヘッダーを送信
  String header = "POST /api/notify HTTP/1.1\r\n";
  header += "Host: notify-api.line.me\r\n";
  header += "Authorization: Bearer " + String(token) + "\r\n";
  header += "User-Agent: " + String("M5Stack") + "\r\n";
  header += "Connection: close\r\n";
  header += "Cache-Control: no-cache\r\n";
  header += "Content-Length: " + String(body_length) + "\r\n";
  header += "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n\r\n";

  client.print(header + body);
  Serial.print(header + body);

  //マルチパートの画像データの分割送信
  bool Success_h = false;
  uint8_t line_try = 3;
  while (!Success_h && line_try-- > 0) {
    if (image_size > 0) {
      size_t BUF_SIZE = 1024;       //バッファサイズの指定
      if ( image_data != NULL) {
        uint8_t *p = image_data;    //送信したデータのサイズ
        size_t sz = image_size;     //送信していないデータのサイズ
        while ( p != NULL && sz) {
          if ( sz >= BUF_SIZE) {    //バッファよりも大きい場合は分割する
            client.write( p, BUF_SIZE); //バッファを送信
            p += BUF_SIZE;          //送信したデータのサイズを更新
            sz -= BUF_SIZE;         //送信していないデータのサイズを更新
          } else {
            client.write( p, sz);   //バッファよりも⼩さい場合、残りを送信
            p += sz; sz = 0;
          }
        }
      }
      client.print("\r\n" + body_end);  //データ送信終了
      Serial.print("\r\n" + body_end);

      while ( client.connected() && !client.available()) delay(10);
      if ( client.connected() && client.available() ) {
        String resp = client.readStringUntil('\n');
        httpCode    = resp.substring(resp.indexOf(" ") + 1,
                                     resp.indexOf(" ", resp.indexOf(" ") + 1)).toInt();
        Success_h   = (httpCode == 200);
        Serial.println(resp);
      }
      vTaskDelay(10);
    }
  }
  client.stop();
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

/* Camera設定 */
void setup_camera() {
  camera_config_t config;

  //M5Camera GPIO
  config.pin_d0 = Y2_GPIO_NUM;          /* 画素データ D0 */
  config.pin_d1 = Y3_GPIO_NUM;          /* 画素データ D1 */
  config.pin_d2 = Y4_GPIO_NUM;          /* 画素データ D2 */
  config.pin_d3 = Y5_GPIO_NUM;          /* 画素データ D3 */
  config.pin_d4 = Y6_GPIO_NUM;          /* 画素データ D4 */
  config.pin_d5 = Y7_GPIO_NUM;          /* 画素データ D5 */
  config.pin_d6 = Y8_GPIO_NUM;          /* 画素データ D6 */
  config.pin_d7 = Y9_GPIO_NUM;          /* 画素データ D7 */
  config.pin_xclk = XCLK_GPIO_NUM;      /* メインクロック XCLK  */
  config.pin_pclk = PCLK_GPIO_NUM;      /* 画素読み出しクロック PCLK */
  config.pin_vsync = VSYNC_GPIO_NUM;    /* 垂直同期信号 VSYNC */
  config.pin_href = HREF_GPIO_NUM;      /* 水平同期信号 HREF  */
  config.pin_sscb_sda = SIOD_GPIO_NUM;  /* SCCB SDA  */
  config.pin_sscb_scl = SIOC_GPIO_NUM;  /* SCCB SCL  */
  config.pin_pwdn = PWDN_GPIO_NUM;      /* パワーダウンモード信号 */
  config.pin_reset = RESET_GPIO_NUM;    /* リセット信号 */
  config.xclk_freq_hz = 20000000;       /* メインクロックXCLK 周波数 */
  config.ledc_channel = LEDC_CHANNEL_0; /* XCLKのLEDC(PWM)チャンネルの設定  */
  config.ledc_timer = LEDC_TIMER_0;     /* XCLKのLEDC(PWM)タイマ-の設定*/

  //M5Camera Image Format
  config.pixel_format = PIXFORMAT_JPEG;   /*画像フォーマット*/
  config.jpeg_quality = 6;                /*JPEG圧縮の品質*/

  config.frame_size = FRAMESIZE_VGA;       /*画像のサイズ*/
  config.fb_count = 1;                     /*フレームバッファサイズ*/

  esp_err_t err = esp_camera_init(&config);  /*カメラの初期化*/
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  sensor_t * s = esp_camera_sensor_get();     /*カメラの撮影*/

}
