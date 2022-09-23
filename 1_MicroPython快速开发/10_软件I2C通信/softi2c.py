'''
 * @文件： softi2c.py
 * @作者： 百问网开发团队
 * @功能： 使用火龙果(Pitaya Lite)开发板上的模拟I2C与EEPROM通信
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
import time
from machine import Pin,SoftI2C

# 定义写EEPROM函数
def wirte_eeprom(devaddr, memaddr, data):
    i2c.writeto_mem(devaddr, memaddr, data)
    print("Write data:", data)

# 定义读EEPROM函数
def read_eeprom(devaddr, memaddr, size):
    data = i2c.readfrom_mem(devaddr, memaddr, size)
    print("Read data:", data)
    return data

# 定义清空EEPROM函数
def clean_eeprom(devaddr, memaddr, size):
    zerobuf = b"00"
    for i in range(memaddr, size):   
        i2c.writeto_mem(devaddr, i, zerobuf)
        time.sleep_ms(10)
    print("Clean eeprom")

# 主函数
if __name__  == '__main__':
    # 构造softi2c对象,参数分别为：时钟引脚、数据引脚、频率、超时时间
    i2c = SoftI2C(scl=Pin('PC6'), sda=Pin('PC7'), freq=10000, timeout=50000)
    # 扫描总线上的I2C设备
    devaddr = i2c.scan()
    print("Devices on the I2C bus:", devaddr)
    
    b1 = bytearray(4)
    print(i2c.readfrom(devaddr[0], 10))
    return
    
    # 准备按键
    key = Pin('PA0', mode=Pin.IN_PULLUP)
    key_flag = 0
    while True:
        if (key.value() == 0) and (key_flag != 2): # 按键首次按下
            time.sleep_ms(200)
            if key.value() == 0:    # 延时200毫秒,消除抖动
                key_flag = 1        # key_flag=1,表示短按
                time.sleep_ms(800)
                if key.value() == 0:# 延时800毫秒,再次检测
                    key_flag = 2    # key_flag=2,表示长按
 
        # 根据按键标志key_flag,决定写EEPROM还是清空EEPROM
        if key_flag == 1:
            wirte_eeprom(devaddr[0], 0, b"pitaya")
            key_flag = 0
        elif key_flag == 2:
            clean_eeprom(devaddr[0], 0, 10)
            time.sleep_ms(500)
            key_flag = 0
        
        # 读EEPROM
        read_eeprom(devaddr[0], 0, 6)
        time.sleep_ms(200)        
                 
   