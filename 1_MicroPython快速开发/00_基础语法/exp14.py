x = 10       # 在函数外面,定义的变量,是全局变量,大家都可以访问

def func1():  
    y = 30    # 在函数里面,定义的变量,是局部变量,只能当前函数内部访问
    global z  # 在函数里面,使用global声明变量是一个全局变量,大家都可以访问
    z = 50   
    print("fun1 x =", x)
    print("fun1 y =", y)
    print("fun1 z =", z)

def func2():  
    y = 40
    print("fun2 x =", x) # 打印来在函数外,定义的全局变量
    print("fun2 y =", y) # 打印自己定义的,局部变量
    print("fun2 z =", z) # 打印global声明的全局变量

func1()    # 调用函数1 
func2()    # 调用函数2

