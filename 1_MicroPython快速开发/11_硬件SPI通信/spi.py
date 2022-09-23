'''
 * @文件： spi.py
 * @作者： 百问网开发团队
 * @功能： 使用火龙果(Pitaya Lite)开发板上的硬件SPI读取Flash的ID
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
from machine import Pin,SPI

# 定义读ID操作函数
def Read_ID():
    cs.low()                           # 片选中Flash              
    spi.write(bytearray([0x90,0,0,0])) # 依次发送0x90、0、0四位数据  
    buf = list(spi.read(2))            # 接收两位数据 
    cs.high()                          # 取消片选Flash 
    print(hex(buf[0]))                 # 厂商ID,默认值：0xEE
    print(hex(buf[1]))                 # 设备ID,默认值：0x16
    
# 主函数
if __name__  == '__main__':
    # 构造spi对象,参数分别为：spi的id、SCK时钟速率、极性、相位、数据格式
    spi=SPI(0, baudrate=1000, polarity=0, phase=0, firstbit=SPI.MSB)
    # 构造Pin对象,作为片选引脚
    cs=Pin('PA4', mode=Pin.OUT_PUSHPULL, value=1)
    # 读取Flash的ID
    Read_ID()
