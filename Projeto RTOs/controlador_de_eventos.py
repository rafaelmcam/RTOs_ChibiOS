import serial
import time

with serial.Serial('/dev/ttyUSB0', 115200) as ser:
    time.sleep(2)
    print("Envie um dos seguintes números para indicar um evento")
    print("1 -> Evento Carro 2A VIA")
    print("2 -> Evento Pedestre")
    print("3 -> Evento Ambulância Via Principal")
    print("4 -> Evento Ambulância Via Principal Passou")
    print("5 -> Evento Ambulância 2A VIA")
    print("6 -> Evento Ambulância 2A VIA Passou")
    while True:
        mandar = input();
        mandar_c = bytes(mandar, "utf-8")
        n = ser.write(mandar_c)
        #time.sleep(0.2)
