## Copyright (c) 2020 aNoken 

from Maix import GPIO
from fpioa_manager import fm, board_info
from machine import UART

# M5StickVのGroveポートG34/G35をUARTで初期化
fm.register(35, fm.fpioa.UART2_TX, force=True)
fm.register(34, fm.fpioa.UART2_RX, force=True)
uart_Port = UART(UART.UART2, 115200,8,0,0, timeout=1000, read_buf_len= 4096)
cnt=0

# 数値データを文字データに変換して、送信する
while True:
    moji=str(cnt)+"\n"
    uart_Port.write(moji)
    time.sleep(1.0)
    cnt=cnt+1

uart_Port.deinit()
del uart_Port
