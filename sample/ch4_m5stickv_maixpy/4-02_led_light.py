## Copyright (c) 2020 aNoken

import lcd
from fpioa_manager import *
from Maix import GPIO
from board import board_info

fm.register(board_info.BUTTON_A, fm.fpioa.GPIO1)
but_a=GPIO(GPIO.GPIO1, GPIO.IN, GPIO.PULL_UP)

fm.register(board_info.BUTTON_B, fm.fpioa.GPIO2)
but_b = GPIO(GPIO.GPIO2, GPIO.IN, GPIO.PULL_UP)

fm.register(board_info.LED_W, fm.fpioa.GPIO3)
led_w = GPIO(GPIO.GPIO3, GPIO.OUT)
led_w.value(1) # LED は0で点灯、1で消灯

fm.register(board_info.LED_R, fm.fpioa.GPIO4)
led_r = GPIO(GPIO.GPIO4, GPIO.OUT)
led_r.value(1) # LED は0で点灯、1で消灯

fm.register(board_info.LED_G, fm.fpioa.GPIO5)
led_g = GPIO(GPIO.GPIO5, GPIO.OUT)
led_g.value(1) # LED は0で点灯、1で消灯

fm.register(board_info.LED_B, fm.fpioa.GPIO6)
led_b = GPIO(GPIO.GPIO6, GPIO.OUT)
led_b.value(1) # LED は0で点灯、1で消灯

lcd.init()
while(True):
    #AボタンでLED白点灯
    if but_a.value() == 0:
        led_w.value(0)
        led_r.value(1)
        led_g.value(1)
        led_b.value(1)

    #BボタンでLED赤点灯
    elif but_b.value()== 0:
        led_w.value(1)
        led_r.value(0)
        led_g.value(1)
        led_b.value(1)

    else:
        led_w.value(1)
        led_r.value(1)
        led_g.value(1)
        led_b.value(1)

