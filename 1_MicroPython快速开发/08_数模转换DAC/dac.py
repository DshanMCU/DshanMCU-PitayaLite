'''
 * @文件： dac.py
 * @作者： 百问网开发团队
 * @功能： 使用火龙果(Pitaya Lite)开发板的DAC输出指定电压
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
from machine import Pin,DAC

# 输出电压函数,将传入电压转化为对应值写入DAC
def output_voltage(dac, vol):
    dac.write_u16(int(65535/3.3*vol))
    print("The output voltage:", vol)
    
# 构造dac对象,参数为：引脚
dac0 = DAC(Pin('PA4'))

# 调用函数输出指定电压
output_voltage(dac0, 0.8)

    


