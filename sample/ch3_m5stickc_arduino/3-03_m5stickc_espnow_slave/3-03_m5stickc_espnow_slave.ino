// Copyright (c) 2020 aNoken

#include <esp_now.h>
#include <WiFi.h>
#include <M5StickC.h>
void InitESPNow();                //ESP-NOWでMaster上のESP-Now を初期化する関数
/* モータの速度を格納する変数 */
int servo_l = 0;
int servo_r = 0;
/* I2Cのモータのアドレス */
const int motorL_adr = 0x60;
const int motorR_adr = 0x64;

/* ESP-Nowの変数 */
#define CHANNEL 1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0
esp_now_peer_info_t slave;

//Motor Driver Processing
void motor_drive_i2c_control(int motor_adr, int speed, byte data1) {
  byte regValue = 0x80;
  regValue = abs(speed);
  if (regValue > 100) regValue = 100;
  regValue = regValue << data1;
  if (speed < 0) regValue |= 0x01;    //reverse rotation
  else           regValue |= 0x02;    //Normal rotation

  Wire.beginTransmission(motor_adr);
  Wire.write(0x00);
  Wire.write(regValue);
  Wire.endTransmission(true);
}


void setup() {
  M5.begin();                         //M5StickCを初期化
  M5.Lcd.setRotation(3);            //画面の向きを指定
  M5.Lcd.setTextFont(1);              //文字の大きさを指定
  M5.Lcd.setCursor(0, 0, 4);         //文字の先頭位置を指定
  M5.Lcd.println("Slave");
  WiFi.mode(WIFI_AP);       //Step1
  configDeviceAP();         //Step2
  Serial.print("AP MAC:"); 
  Serial.println(WiFi.softAPmacAddress());
  InitESPNow();         //Step3
  esp_now_register_recv_cb(OnDataRecv);   //Step4
  Wire.begin(32, 33, 10000);     //I2C Setting
  motor_drive_i2c_control(motorL_adr, (servo_l), 0x02);
  motor_drive_i2c_control(motorR_adr, (servo_r), 0x02);
}

//Step5
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  //  Servo_write_us(servo_l, data[0]); //停止する
  //  Servo_write_us(servo_r, data[1]); //停止する
  if (data_len > 0) {
    M5.Lcd.setCursor(0, 30, 4);
    servo_l = data[0];
    servo_r = data[1];
    if (servo_l > 128)servo_l = servo_l - 256;
    if (servo_r > 128)servo_r = servo_r - 256;

    M5.Lcd.printf("%03d,%03d\n", servo_l, servo_r );
    Serial.printf("%03d,%03d\n", servo_l, servo_r);

    motor_drive_i2c_control(motorL_adr, (servo_l), 0x02);
    motor_drive_i2c_control(motorR_adr, (servo_r), 0x02);
  }
  vTaskDelay(1);
}
void loop() {
  M5.update();
  vTaskDelay(20);
}

/**/
void configDeviceAP() {     
  char* SSID = "your_ssid_1";
  bool result = WiFi.softAP(SSID, "your_password", CHANNEL, 0);
  M5.Lcd.setCursor(0, 60, 4);
  if (!result) {
    M5.Lcd.println("AP Config failed.");
    Serial.println("AP Config failed.");

  } else {
    M5.Lcd.println("AP: " + String(SSID));
    Serial.println("AP: " + String(SSID));
  }
}

/*ESP-NOWでMaster上のESP-Now を初期化する関数*/
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {   //ESPNOWを初期化実施
    Serial.println("ESPNow Init Success");
  }
  else {                           //初期化に失敗した場合
    Serial.println("ESPNow Init Failed");
    ESP.restart();                  //ESPを再起動
  }
}
