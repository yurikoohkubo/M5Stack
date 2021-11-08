// Copyright (c) 2020 aNoken

#include <M5Stack.h>
TFT_eSprite *Spr;
uint16_t front_col = BLACK;
uint16_t back_col = YELLOW;

/* 顔の表⽰を⾏う関数 */
void draw_face(float open) { //引数はお⼝の開き具合
  Spr = new TFT_eSprite(&M5.Lcd);
  Spr->setColorDepth(8);
  Spr->createSprite(320, 240);
  Spr->setBitmapColor(front_col, back_col);
  Spr->fillSprite(back_col);  //背景を塗る。

  //左眉
  Spr->fillTriangle(190, 70, 190, 50,
                    280 + random(10), 25 + random(15), front_col);
  //右眉
  Spr->fillTriangle(130, 70, 130, 50,
                    50 + random(10), 25 + random(15), front_col);
  //左目
  Spr->fillCircle(90 + random(5), 93 + random(5),
                  25 , front_col );
  //右目
  Spr->fillCircle(230 + random(5), 93 + random(5),
                  25 , front_col );
  //お⼝
  Spr->fillTriangle(133, 188 - open / 2, 133 + 60, 188 - open / 2,
                    133 + 60, 188 + open / 2, front_col);
  Spr->fillTriangle(133, 188 - open / 2, 133, 188 + open / 2,
                    133 + 60, 188 + open / 2, front_col);

  //スプライトを表示する
  Spr->pushSprite(0, 0);
}
void setup() {
  M5.begin();
}
void loop() {
  M5.update();
  draw_face(0); //顔表示を呼び出し
  vTaskDelay(50);
}
