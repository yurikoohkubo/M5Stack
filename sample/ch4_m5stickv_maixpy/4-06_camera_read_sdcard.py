## Copyright (c) 2020 aNoken

import sensor,image,lcd

lcd.init()
lcd.rotation(2)

fname="save_1.jpg"
img_read = image.Image(fname)
lcd.display(img_read)
