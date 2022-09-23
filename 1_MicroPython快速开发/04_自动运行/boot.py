'''
 * @文件： boot.py
 * @作者： 百问网开发团队
 * @功能： 火龙果(Pitaya Lite)开机启动程序
          通过按键判断是启动main.py,还是REPL
 * @版本： 0.1
 * @日期： 2022-06-20
 * @Copyright (c) 2022 深圳百问网科技有限公司
'''

import os
from machine import Pin

# 构造key对象
key = Pin('PA0', mode=Pin.IN_PULLUP)  

# 获取key值
if key.value() == 0:                 # 按键如果按下
    print("Welcome to Pitaya Lite")  # 打印欢迎界面提示,进入REPL
else:                                # 按键如果没按下
    if "main.py" in os.listdir():    # 判断main.py是否存在
        print("Run main.py")         # 打印运行提示
        import main                  # 导入main模块运行
    else:
        print("Can not find main.py")# 打印错误提示
