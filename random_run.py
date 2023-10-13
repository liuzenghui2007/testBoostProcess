import time
import random

# 获取当前时间字符串
current_time = time.strftime("%Y-%m-%d_%H-%M-%S")

# 创建一个以当前时间命名的文本文件
file_name = f"D:\\{current_time}.txt"
with open(file_name, "w") as file:
    file.write("This is a text file created by the Python script.")
    
# 生成随机等待时间，介于10到50秒之间
wait_time = random.randint(5, 30)
print(f"Python script running for {wait_time} seconds...")
time.sleep(wait_time)


