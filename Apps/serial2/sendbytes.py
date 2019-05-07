import serial
import time

with serial.Serial('/dev/ttyUSB0', 115200) as ser:
    time.sleep(3)
    print("Cifra de Cesar")
    while True:
        #print("Novo Loop")
        #mandar = "abc"
        mandar = input();
        mandar_c = bytes(mandar, "utf-8")
        n = ser.write(mandar_c)
        x = ser.read(len(mandar))
        print(x)
        #time.sleep(0.2)
