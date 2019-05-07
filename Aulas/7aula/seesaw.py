import serial
import time
from itertools import chain

MIN, MAX, STEP = 0, 100, 2
REPT, DELAY = 5, 0.02

pwm_values = bytes(x*x//MAX for x in
                   chain(range(MIN, MAX, STEP), range(MAX, MIN, -STEP)))
with serial.Serial("/dev/ttyUSB0", 9600) as ser:
    for n in range(REPT):
        for c in pwm_values:
            ser.write(bytes([c]))
            time.sleep(DELAY)

print("done!")
