## Copyright (c) 2020 aNoken

import sensor,image,lcd
import KPU as kpu

# LCDとカメラの初期化
lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_vflip(1)
sensor.run(1)
classes = ["class-1","class-2","class-3"]

#YOLOv2のKmodelを読み込む
task = kpu.load("my_yolo.kmodel")

#アンカーを設定します
anchor = (0.57273, 0.677385, 1.87446, 2.06253, 3.33843,
        5.47434, 7.88282, 3.52778, 9.77052, 9.16828)

#YOLOv2を初期化する
a = kpu.init_yolo2(task, 0.2, 0.3, 5, anchor)

while(True):
    img = sensor.snapshot()
    #YOLOv2 で物体検出を行います。
    code = kpu.run_yolo2(task, img)
    #物体がある場合には矩形で囲みます
    if code:
        for i in code:
            a=img.draw_rectangle(i.rect(),color = (0, 255, 0))
            a = img.draw_string(i.x(),i.y(), classes[i.classid()],
            color=(255,0,0), scale=1)
        a = lcd.display(img)
    else:
        a = lcd.display(img)
a = kpu.deinit(task)
