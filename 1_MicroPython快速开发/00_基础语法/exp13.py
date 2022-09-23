def max_num(a, b=0):   # 设置b为默认参数
    "功能: 打印两个数字中较大的一个"
    if a>b:
        print(a, "is maximun")
    elif a<b:
        print(b, "is maximun")
    else:
        print(a, "is equal to", b)


max_num(15, 17)      # 正常调用函数，为默认顺序
max_num(-27)         # 只传入参数a,参数b为默认值
max_num(b=-7, a=-10) # 指定传给哪一个参数

