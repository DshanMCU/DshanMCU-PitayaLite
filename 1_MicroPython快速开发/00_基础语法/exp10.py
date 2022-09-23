x=10

while True: # 恒为Ture,无限循环
    x = x-1 
    if x%2==0:   # 如果除2余0,说明是偶数
        continue # 跳过本次循环
    else:
        print("x value =:", x)

    if x<0:      # 如果x为负数
        break    # 结束循环

