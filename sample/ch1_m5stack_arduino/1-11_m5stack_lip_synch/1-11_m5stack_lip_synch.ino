// Copyright (c) 2020 aNoken

//以下のURL（AquesTalkの作者のHP）から「SampleTTS.zip」をダウンロ
//ードし、「AquesTalkTTS.cpp」と「AquesTalkTTS.h」を入手しましょう。
//http://blog-yama.a-quest.com/?eid=970195

#include <M5Stack.h>
#include "AquesTalkTTS.h"
TFT_eSprite *Spr;
uint32_t front_col = BLACK;
uint32_t back_col = YELLOW;

/*　顔の表示を行う関数　*/
void draw_face(float open) { //引数はお口の開き具合
  Spr = new TFT_eSprite(&M5.Lcd);
  Spr->setColorDepth(8);
  Spr->createSprite(320, 240);
  Spr->setBitmapColor(front_col, back_col);
  Spr->fillSprite(back_col);  //背景を塗る。
  Spr->fillTriangle(190, 70, 190, 50,
                    280 + random(10), 25 + random(15), front_col); //左眉
  Spr->fillTriangle(130, 70, 130, 50,
                    50 + random(10), 25 + random(15), front_col); //右眉
  Spr->fillCircle(90 + random(5), 93 + random(5),
                  25 , front_col );//左目
  Spr->fillCircle(230 + random(5), 93 + random(5),
                  25 , front_col );//右目
  Spr->fillTriangle(133, 188 - open / 2, 133 + 60, 188 - open / 2,
                    133 + 60, 188 + open / 2, front_col); //口
  Spr->fillTriangle(133, 188 - open / 2, 133, 188 + open / 2,
                    133 + 60, 188 + open / 2, front_col); //口
  Spr->pushSprite(0, 0);     //スプライトを表示する
}
void drawLoop(void *args) {
  Spr->setColorDepth(8);
  Spr->createSprite(320, 240);
  Spr->setBitmapColor(front_col, back_col);
  for (;;)  {
	/* 音量からお口の大きさを求める*/
    int level = TTS.getLevel();
    float open = level / 250.0;
	/* お口をあける量を指定 */
    draw_face(open);
    vTaskDelay(50);
  }
}
void setup() {
  M5.begin();
  Spr = new TFT_eSprite(&M5.Lcd);
  int iret = TTS.create(NULL);
  if (iret)    Serial.println("ERR: TTS_create():");
	/* お顔を表示させるスレッドの生成 */
  xTaskCreatePinnedToCore(drawLoop,"drawLoop",4096,NULL,1,NULL,0);
}
void loop() {
  M5.update();
  if (M5.BtnA.wasPressed())  TTS.play("okawari-", 80);
  vTaskDelay(50);
}


