import serial
import time 

ser = serial.Serial('/dev/cu.usbmodem101', 115200, timeout=1)
time.sleep(2)

while True:
    line = ser.readline().decode('utf-8').strip()

    try:
        ax, ay, az, gx, gy, gz = map(int, line.split(","))

        print(ax, ay, az, gx, gy, gz)

    except ValueError:      #Except do komunikatow pomocniczych (BMI OK, BMI Calibrating itp. )
        print("El negro")
        pass