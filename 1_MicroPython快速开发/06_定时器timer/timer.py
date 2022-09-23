'''
 * @文件： timer.py
 * @作者： 百问网开发团队
 * @功能： 使用火龙果(Pitaya Lite)开发板的定时器实现LED闪烁和打印
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
from machine import Pin
from machine import Timer

led = Pin('PA1', mode=Pin.OUT_PUSHPULL) 

# 定义一个函数,供定时器回调使用
def t0_callback(self):
    led(1-led())
    print(led())

# 构造timer对象,参数分别为：定时器id、定时器模式、定时器周期、回调函数
t0 = Timer(0, mode=Timer.PERIODIC, period=1000, callback=t0_callback)

# 之后无需做任何操作,定时器到了设定的时间,则会调用回调函数
while True:
    pass




