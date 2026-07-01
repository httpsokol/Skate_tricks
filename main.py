import serial

ser = serial.Serial('/dev/tty.usbmodem1101', 115200)

while True:
    line = ser.readline().decode().strip()
    print(line)

print("Hello world!")