import serial
import time

R_fixo_conectado_a_VCC = 220
R_anterior = 0


with serial.Serial("/dev/ttyUSB0", 115200) as ser:
    while True:
        n = ser.read()[0] + (ser.read()[0] << 8)
        R = R_fixo_conectado_a_VCC*n/(1024-n)
        if (abs(R_anterior-R)>0.1*R):
            print("Resistência Conectada ao Ground: {:.2f} omhs".format(R))
        R_anterior = R
