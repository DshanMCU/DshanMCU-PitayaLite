'''
 * @文件： pwm.py
 * @作者： 百问网开发团队
 * @功能： 实现火龙果(Pitaya Lite)开发板上的蓝色LED灯(PA1)呼吸效果
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
import time
from machine import Pin,PWM

# 构造pwm对象,参数分别为：引脚、频率、占空比
led = PWM(Pin('PA1'), freq=500, duty=100) # 设置初始频率和占空比

while True:                           # 无限循环
    for duty in range(0, 1000, 100):  # 占空比从0递增到1000,每次增加100
        led.duty(duty)                # 重新设置占空比
        time.sleep_ms(100)            # 保持这个占空比一段时间
    for duty in range(1000, 0, -100): # 占空比从1000减到0,每次减小100
        led.duty(duty)                # 重新设置占空比
        time.sleep_ms(100)            # 保持这个占空比一段时间




