'''
 * @文件： led.py
 * @作者： 百问网开发团队
 * @功能： 点亮火龙果(Pitaya Lite)开发板上的蓝色LED灯(PA1)
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
from machine import Pin  # 只导入machine模块中的Pin

# 构造pin对象,参数分别为：引脚号、引脚模式、初始电平
led = Pin('PA1', mode=Pin.OUT_PUSHPULL, value=1)
# 使用low()输出低电平(亮灯)，high()输出高电平(灭灯)
led.low()