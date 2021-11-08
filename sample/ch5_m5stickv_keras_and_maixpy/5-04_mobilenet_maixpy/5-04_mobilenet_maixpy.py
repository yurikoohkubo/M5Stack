## Copyright (c) 2020 aNoken

import sensor, image, lcd, time
import KPU as kpu

# LCDとカメラの初期化
lcd.init()
lcd.rotation(2)
lcd.clear()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.run(1)

#クラスを定義する
labels = ['001_class', '002_class', '003_class']

# kmodelの読み込み
task = kpu.load("my_mbnet.kmodel")
clock = time.clock()

while(True):
    img = sensor.snapshot()
    clock.tick()
    fmap = kpu.forward(task, img)	#KPU でCNNを演算します
    fps=clock.fps()
    plist=fmap[:]					#オブジェクトのクラスごとの確率を取り出します
    pmax=max(plist)					#最も確率の高いものを取り出します
    max_index=plist.index(pmax)
    a = lcd.display(img)
    #結果をM5StickVに描画します
    lcd.draw_string(10, 96, "%.2f:%s"%(pmax, labels[max_index].strip()))
    print(fps)
a = kpu.deinit(task)

