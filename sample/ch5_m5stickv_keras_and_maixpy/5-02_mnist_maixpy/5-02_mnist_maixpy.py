## Copyright (c) 2020 aNoken
## This Code is Maixpy

import sensor,lcd,image
import KPU as kpu
# LCDとカメラの初期化
lcd.init()
lcd.rotation(2)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
# kmodelの読み込み
task = kpu.load("mnist.kmodel")
sensor.run(1)

while True:
    img = sensor.snapshot()
    lcd.display(img) 			#カメラ画像を表示します
    img1=img.to_grayscale(1) 	#グレースケールに変換します
    img2=img1.resize(28,28) 	#28x28 にリサイズします
    a=img2.invert() 			#反転します
    a=img2.strech_char(1) 		#前処理を行います
    lcd.display(img2,oft=(120,32)) 	#28x28 画像を表示します
    a=img2.pix_to_ai(); 			#AIデータに変換します
    fmap=kpu.forward(task,img2) 	#KPU でCNNを演算します
    plist=fmap[:] 					#10 個の数字の確率を取り出します
    pmax=max(plist) 				#最も確率の高いものを取り出します
    max_index=plist.index(pmax) 	#数字を求めます
    #結果をM5StickVに描画します
    lcd.draw_string(0,0,"%d: %.3f"%(max_index,pmax),lcd.WHITE,lcd.BLACK)




