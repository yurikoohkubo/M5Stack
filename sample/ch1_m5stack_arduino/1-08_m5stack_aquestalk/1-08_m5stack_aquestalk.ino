// Copyright (c) 2020 aNoken

//以下のURL（AquesTalkの作者のHP）から「SampleTTS.zip」をダウンロ
//ードし、「AquesTalkTTS.cpp」と「AquesTalkTTS.h」を入手しましょう。
//http://blog-yama.a-quest.com/?eid=970195

#include "AquesTalkTTS.h"
#include <M5Stack.h>
#define AQUESTALK_KEY "xxxx"

void setup() {
  M5.begin();                       //M5Stackを初期化
  M5.Power.begin();                 //M5Stackのバッテリ初期化

  //AqeusTalkを初期化します
  int iret = TTS.create(AQUESTALK_KEY);

  //AqeusTalkでテキストメッセージを音声出力します
  TTS.play("okawari-", 80);
}
void loop() {
  M5.update();                      //M5Stackの内部状態を更新
}
