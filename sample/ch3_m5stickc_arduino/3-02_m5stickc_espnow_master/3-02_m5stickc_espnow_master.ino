// Copyright (c) 2020 aNoken

#include <esp_now.h>
#include <WiFi.h>
#include <M5StickC.h>
void InitESPNow();                //ESP-NOWでMaster上のESP-Now を初期化する関数
void ScanForSlave();              //ESP-NOWでSlaveのESP32を探す関数
bool manageSlave();               //ESP-NOWでSlaveのESP32を追加する関数
void deletePeer();                //ESP-NOWでpeerを削除する関数

/* モータの速度を格納する変数 */
int servo_l = 0;
int servo_r = 0;

/* ESP-Nowの変数 */
#define CHANNEL 1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0
esp_now_peer_info_t slave;

void sendData() {                 //MasterからSlaveへデータ送信を開始する
  uint8_t data[2];
  data[0] = servo_l;  data[1] = servo_r;
  const uint8_t *peer_addr = slave.peer_addr;
  esp_err_t result = esp_now_send(peer_addr, data, 2);  //ESPNOWで送信

  M5.Lcd.setCursor(0, 60, 4);
  Serial.print("send status: ");
  if (result == ESP_OK){   
    M5.Lcd.println("success");M5.Lcd.println("success");
    }
  else  {  M5.Lcd.println("error");M5.Lcd.println("error");}
}
void setup() {
  M5.begin();                           //M5StickCを初期化
  M5.Lcd.setRotation(3);                //画面の向きを指定
  M5.Lcd.setCursor(0, 30, 4);           //文字の先頭位置を指定
  M5.Lcd.println("Master");
  WiFi.mode(WIFI_STA);                    //Wi-Fiをステーションモードで起動
  InitESPNow();                           //ESP-Now を初期化する
}
void loop() {
  M5.update();                        //M5StickCの内部状態を更新
  /* M5のボタンで動作を切り替える */
  if (M5.BtnA.isPressed()) {
    servo_l = 100; servo_r = 100;     //前進のモータ速度を指定
  }
  else if (M5.BtnB.isPressed()) {
    servo_l = 100; servo_r = -100;    //その場旋回のモータ速度を指定
  }
  else  {
    servo_l = 0; servo_r = 0;         //停止のモータ速度を指定
  }
  M5.Lcd.setCursor(0, 30, 4);
  M5.Lcd.printf("%03d,%03d\n", servo_l, servo_r);
  Serial.printf("%03d,%03d\n", servo_l, servo_r);

  if (slave.channel == CHANNEL) {     //SLAVEが見つかっている場合の処理
    bool isPaired = manageSlave();    //SlaveのESP32を追加する関数
    if (isPaired) {
      sendData();                     //ESPNOWでデータ送信する
    } else {
      M5.Lcd.setCursor(0, 60, 4);
      M5.Lcd.println("Slave pair failed!");
      Serial.println("Slave pair failed!");
    }
  }
  else {                              //SLAVEが見つかっていない場合の処理
    ScanForSlave();                   //SlaveのESP32を探す
  }
  vTaskDelay(20);
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

/*ESP-NOWでSlaveのESP32を探す関数 */
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();     // Wi-Fiを探索
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      //検出された各デバイスのSSIDとRSSIを出力
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      if (PRINTSCANRESULTS) {
        Serial.print(i + 1);       Serial.print(": ");
        Serial.print(SSID);        Serial.print(" (");
        Serial.print(RSSI);        Serial.print(")");
        Serial.println("");
      }
      delay(10);
      // 現在のデバイスが「your_ssid」で始まるかどうかを確認します

      if (SSID.indexOf("your_ssid") == 0) {
        // 対象のSSID
        Serial.println("Found a Slave.");
        Serial.print(i + 1);    Serial.print(": "); 
        Serial.print(SSID);     Serial.print(" ["); 
        Serial.print(BSSIDstr); Serial.print("]"); 
        Serial.print(" (");     Serial.print(RSSI); 
        Serial.print(")");      Serial.println("");
        //BSSIDから、スレーブのMacアドレスを取得
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x%c",
            &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = CHANNEL;  //チャンネルを選ぶ
        slave.encrypt = 0;        //暗号化なし
        slaveFound = 1;           //この例ではスレーブを1つだけに指定。
        break;
      }
    }
  }

  if (slaveFound) {
    Serial.println("Slave Found, processing..");
  } else {
    Serial.println("Slave Not Found, trying again.");
  }
  WiFi.scanDelete();      // ramをクリーンアップ
}

/*ESP-NOWでpeerを削除する関数*/
void deletePeer() {
  const esp_now_peer_info_t *peer = &slave;
  const uint8_t *peer_addr = slave.peer_addr;

   //peerを削除しステータスを取得
  esp_err_t delStatus = esp_now_del_peer(peer_addr);   
  Serial.print("Slave Delete Status: ");
  if (delStatus == ESP_OK) {
    Serial.println("Success");      //削除を成功
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
    Serial.println("ESPNOW Not Init");
  } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

/* ESP-NOWでSlaveのESP32を追加する関数 */
bool manageSlave() {
  //スレーブがマスターとすでにペアリングされているかどうかを確認します。
//そうでない場合は、スレーブをマスターとペアにします

  if (slave.channel == CHANNEL) {
    if (DELETEBEFOREPAIR) {
      deletePeer();
    }
    Serial.print("Slave Status: ");
    const esp_now_peer_info_t *peer = &slave;
    const uint8_t *peer_addr = slave.peer_addr;
    //ピアが存在するかどうかを確認します
    bool exists = esp_now_is_peer_exist(peer_addr);
    if ( exists) {    // スレーブはすでにペアリングされている場合
      Serial.println("Already Paired");
      return true;
    } else {
      // ペアリングを追加してステータスを取得する
      esp_err_t addStatus = esp_now_add_peer(peer);
      if (addStatus == ESP_OK) {
        // 
        Serial.println("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
    }
  } else {        // 処理するスレーブが見つからない場合
    Serial.println("No Slave found to process");
    return false;
  }
}
