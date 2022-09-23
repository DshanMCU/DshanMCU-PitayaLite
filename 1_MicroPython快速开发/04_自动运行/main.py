'''
 * @文件： main.py
 * @作者： 百问网开发团队
 * @功能： 闪烁火龙果(Pitaya Lite)开发板上的蓝色LED灯(PA1)
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
import time
from machine import Pin  

led = Pin('PA1', mode=Pin.OUT_PUSHPULL, value=1)

while True:            # 无限循环
    led.low()          # 灯亮
    time.sleep_ms(200) # 延时200毫秒
    led.high()         # 灯灭
    time.sleep_ms(200) # 延时200毫秒


