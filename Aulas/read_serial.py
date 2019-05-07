import serial
import time
import sys

VELOCIDADE_SERIAL = 115200 if (len(sys.argv)==1) else sys.argv[1]
    
#VELOCIDADE_SERIAL = 115200
#VELOCIDADE_SERIAL = 9600

print("Velocidade da Serial Utilizada = " + str(VELOCIDADE_SERIAL))
with serial.Serial('/dev/ttyUSB0', VELOCIDADE_SERIAL) as ser:
    time.sleep(2)
    while True:
        x = ser.readline()
        #print(x)
        print(x.decode("utf-8"))
