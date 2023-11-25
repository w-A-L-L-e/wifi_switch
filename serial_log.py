# log directly in terminal using python instead of arduino gui
import serial
import os

#ser = serial.Serial("/dev/cu.usbserial-0001", 115200)
ser = serial.Serial("/dev/cu.usbserial-FTB6SPL3", 74880)

while True:
    line = ser.readline()

    try:
        data = line.decode()
        print(f"DATA: '{data}'")
            
    except UnicodeDecodeError:
        print(f"RAW SERIAL: {line}")
        pass

