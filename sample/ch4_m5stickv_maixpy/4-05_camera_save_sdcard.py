## Copyright (c) 2020 aNoken

import sensor, image,lcd,os
from fpioa_manager import fm

#ボタンを初期化
fm.register(board_info.BUTTON_A, fm.fpioa.GPIO1)
but_a=GPIO(GPIO.GPIO1, GPIO.IN, GPIO.PULL_UP)
fm.register(board_info.BUTTON_B, fm.fpioa.GPIO2)
but_b = GPIO(GPIO.GPIO2, GPIO.IN, GPIO.PULL_UP)
is_button_a = 0
is_button_b = 0

#LCDの初期化
lcd.init()
lcd.rotation(2)

#カメラの初期化
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.run(1)

#画像のファイル名を指定
path = "save_"
ext=".jpg"
cnt=0
img_read = image.Image()

print(os.listdir())

while True:
    if is_button_b == 1:
        lcd.display(img_read)

    else :
        img=sensor.snapshot()
        lcd.display(img)

    #Aボタンを押されたら、カウントアップして画像を保存
    if but_a.value() == 0 and is_button_a == 0:
        print("save image")
        cnt+=1
        fname=path+str(cnt)+ext
        print(fname)
        img.save(fname, quality=95)
        is_button_a=1

    if but_a.value() == 1 and is_button_a == 1:
        is_button_a=0

    #Bボタンを押されたら、画像を読み出し
    if but_b.value() == 0 and is_button_b == 0:
        fname=path+str(cnt)+ext
        print(fname)
        img_read = image.Image(fname)
        is_button_b=1

    if but_b.value() == 1 and is_button_b == 1:
        is_button_b=0
