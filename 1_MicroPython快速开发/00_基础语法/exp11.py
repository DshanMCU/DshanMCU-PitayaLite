# 遍历字符串中每个字符
for letter in "MicroPython":
    print("letter is", letter)

# 遍历列表中每个元素
languages = ["Chinese", "Spanish", "English", "Japanese"]
for i in languages:        
   print('Supported languages: ', i)

# 根据范围遍历
for i in range(10, 15):        
   print('i =: ', i)

# 根据索引遍历
languages = ["Chinese", "Spanish", "English", "Japaniese"]
for i in range(len(languages)):        
   print('Supported languages: ', languages[i])
   