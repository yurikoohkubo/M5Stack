// Copyright (c) 2020 aNoken

#include "esp_camera.h"

//"camera_pins.h"は、スケッチ例 ESP32>Camera>CameraWebServer から取得
#define CAMERA_MODEL_M5STACK_WIDE
#include "camera_pins.h"

void setup() {
  Serial.begin(115200);
  setup_camera();                           //Camera設定
}
void loop() {
  uint32_t data_len = 0;
  camera_fb_t * fb = esp_camera_fb_get();   //Camera撮影
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  } else {
    Serial.println("Camera capture OK");
  }
  /*ここに画像を使う処理を追加する*/
  esp_camera_fb_return(fb);                //カメラのバッファメモリを解放

  vTaskDelay(1000);
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
