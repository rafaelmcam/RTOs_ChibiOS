import serial


with serial.Serial("/dev/ttyUSB0", 115200) as ser:
    while 1:
        for i in range(5):
            n = ser.read()[0]
            print("{:x}".format(n))
        print("--------")

