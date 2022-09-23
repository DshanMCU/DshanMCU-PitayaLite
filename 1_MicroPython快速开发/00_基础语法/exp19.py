try: # 尝试打开hello.txt
    f = open("hello.txt", "r")
except : # 错误进入异常处理
    print("Cannot find hello.txt file") 
else:
    print("hello.txt already exists.")
    f.close()

try: # 尝试打开test.txt
    f = open("test.txt", "r")
except : # 错误进入异常处理
    print("Cannot find test.txt file.")
else:
    print("test.txt already exists.")
    f.close()
    