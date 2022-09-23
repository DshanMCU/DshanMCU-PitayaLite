x=10
y=20 

# 仅有if
if x>y:
    print("x>y")

# 有if和else
if x==y:
    print("x==y")
else:
    print("x!=y")

# 多次判断
if x==y:
    print("x==y")
elif x>y:
    print("max:", x)
elif y>x:
    print("max:", y)