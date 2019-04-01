#!/usr/bin/python3
import serial
ser = serial.Serial('/dev/ttyACM0', 115200)

# read from Arduino
input = ser.read()
print ("Read input " + input.decode("utf-8") + " from Arduino")

# write something back
ser.write('test')
