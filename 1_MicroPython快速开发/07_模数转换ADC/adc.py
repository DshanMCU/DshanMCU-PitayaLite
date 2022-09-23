'''
 * @文件： adc.py
 * @作者： 百问网开发团队
 * @功能： 获取火龙果(Pitaya Lite)开发板的按键(PA0)的ADC值
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
import time
from machine import Pin,ADC

# 构造adc对象,参数分别为：引脚号、初始化
adc = ADC(Pin('PA0'), init=True)

while True:
    print(adc.read_u16()) # 获取ADC值并打印
    time.sleep_ms(200)
    

