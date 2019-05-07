import serial
import time

with serial.Serial('/dev/ttyUSB0', 115200) as ser:
    time.sleep(2)
    while True:
        x = ser.read(2)
        print("Received {}".format(x[0] + (x[1] << 8)))
