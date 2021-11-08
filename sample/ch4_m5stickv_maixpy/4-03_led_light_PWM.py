## Copyright (c) 2020 aNoken

import time,math
from machine import Timer,PWM
from fpioa_manager import fm
from board import board_info

#タイマーを設定
tim = Timer(Timer.TIMER0, Timer.CHANNEL0, mode=Timer.MODE_PWM)
#LED赤にPWMを設定
PWM_ch = PWM(tim, freq=500000, duty=0, pin=board_info.LED_R)
cnt=0
while(True):
    #正弦波を生成
    duty_val=math.fabs(math.sin(cnt))*100
    #LEDを正弦波で点灯
    PWM_ch.duty(duty_val)
    cnt=cnt+0.01
    time.sleep_ms(1)

