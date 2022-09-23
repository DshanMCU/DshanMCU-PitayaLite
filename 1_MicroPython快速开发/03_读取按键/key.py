'''
 * @文件： key.py
 * @作者： 百问网开发团队
 * @功能： 获取火龙果(Pitaya Lite)开发板上的用户按键状态(PA0)
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
import time
from machine import Pin  

key = Pin('PA0', mode=Pin.IN_PULLUP) # 此时GPIO模式是上拉输入模式

while True:                # 无限循环
    if key.value() == 0:   # 如果按键按下
        print("key press") # 打印按键按下信息
        time.sleep_ms(200) # 延时200毫秒,避开按键抖动

