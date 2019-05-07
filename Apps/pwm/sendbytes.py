import serial
import time

with serial.Serial('/dev/ttyUSB0', 115200) as ser:
    time.sleep(3)
    while True:
        x = ser.read(1)
        print("Received {}".format(x))
