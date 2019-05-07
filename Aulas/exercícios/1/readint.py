import serial

ant = 0

print("{:^20} |{:^20} |{:^35}".format("Recebido", "Convertido (s)", "Diferença para o anterior(s)"))

with serial.Serial("/dev/ttyUSB0", 115200) as ser:
    while True:
        n = ser.read()[0] + (ser.read()[0] << 8)
        x = 0.00006400097*n
        if ant > x:
            print("{:^20d} {:^20f} {:^35f}".format(n, x, (x-ant + 4.194304)))
        elif ant < x:
            print("{:^20d} {:^20f} {:^35f}".format(n, x,( x- ant)))
        ant = x
