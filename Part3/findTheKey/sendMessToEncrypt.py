import random
from datetime import datetime
import serial
import time

arduino = serial.Serial(port='COM12', baudrate=9600, timeout=.1)


def write(data):
    try:
        while not arduino.writable():
            pass
        arduino.write(data.encode())
    except:
        pass


def preReading():
    try:
        while not arduino.readable():
            pass
        return arduino.readline().decode()
    except:
        return 0


def read():
    try:
        while not arduino.readable():
            pass
        data = ""
        while data == "":
            data = arduino.readline().decode()
        return data
    except:
        return 0


def stringToInt(number):
    n = len(number)
    returnNum = 0
    temp = 1

    for i in range(n):
        if number[n - 1 - i] == '\r' or number[n - 1 - i] == '\n':
            continue

        if number[n - 1 - i] == '1':
            returnNum += temp

        temp *= 2
    return returnNum


def longToBytes(x):
    bits = ""
    if x == 0:
        return "0"

    while x > 0:

        if x % 2 == 0:
            bits += '0'
        else:
            bits += '1'
        x = x // 2

    return bits[::-1]


write(0)
time.sleep(5)
print(preReading())

n = 3839256683
e = 548447537

with open('timeData.txt', 'w') as f1:
    f1.writelines("N,E\n")
    f1.writelines(str(n) + "," + str(e) + "\n")
    f1.writelines("message,signature,duration\n")
    for i in range(1, 10):
        m = random.randrange(1, n)
        t1 = datetime.now()
        write(longToBytes(int(m)))
        c2 = stringToInt(read())
        t2 = int((datetime.now() - t1).total_seconds() * 1000000)
        f1.writelines(str(m) + "," + str(c2) + "," + str(t2) + "\n")
