'''
 * @文件： uart.py
 * @作者： 百问网开发团队
 * @功能： 使用火龙果(Pitaya Lite)开发板的UART1与ESP8266 WIFI模块通信
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''
import time
from machine import UART

# 定义发送AT指令函数
def Send_AT_CMD(cmd):
    ack_msg = b""
    uart.write(cmd + "\r\n")  # 发AT指令,加上回车换行结束
    while True:
        # 读取串口收到数据,并去掉回车换行符
        if uart.any() > 0:
            tmp = uart.read(1).replace(b'\r',b'').replace(b'\n',b' ') # 
            ack_msg = ack_msg + tmp
        # 对收到的数据进行比较判断
        if ack_msg.find(b'ERROR') > -1:
            print(cmd + "ERROR")
            return -1
        elif ack_msg.find(b"OK") > -1:
            print(b"ESP8266 ACK:"+ack_msg)
            return 1

# 主函数
if __name__  == '__main__':
    # 构造uart对象,参数分别为：串口id、波特率、数据位、校验位、停止位
    uart=UART(1, baudrate=115200, bits=8, parity=0, stop=1)

    # 发送AT测试命令
    Send_AT_CMD("AT")
    # 设置ESP8266为Station站点模式 
    Send_AT_CMD("AT+CWMODE_DEF=1")
    # 设置要连接的路由器名字和密码 
    Send_AT_CMD("AT+CWJAP_DEF=\"pitaya\",\"100ask100ask\"")
    # 延时一会,等待连接完成
    time.sleep_ms(500)
    # 查询连接结果
    Send_AT_CMD("AT+CIFSR")
