## Copyright (c) 2020 aNoken

import sensor,image,lcd
#LCDの初期化
lcd.init()
lcd.rotation(2)
#カメラの初期化
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.run(1)
while True:
    #カメラ画像をLCDに表示
    img=sensor.snapshot()
    lcd.display(img)
