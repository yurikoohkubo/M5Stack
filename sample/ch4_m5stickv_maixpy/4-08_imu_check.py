## Copyright (c) 2020 aNoken
from Maix import GPIO
from fpioa_manager import fm, board_info
from machine import I2C

i2c = I2C(I2C.I2C0, freq=400000, scl=28, sda=29)
devices = i2c.scan()
print("I2C G28, G29")
print(devices)

fm.register(25, fm.fpioa.GPIO7)
i2c_cs = GPIO(GPIO.GPIO7, GPIO.OUT)
i2c_cs.value(1)
i2c = I2C(I2C.I2C0, freq=400000, scl=26, sda=27)
devices = i2c.scan()
print("I2C G26, G27")
print(devices)


