# -*- coding: utf-8 -*
import serial
import time


class lidarControl:
    ser = serial.Serial("/dev/ttyS0", 115200)
    print('INFO: Lidar init done');

    #f=open("lidarDump.txt",a)

    def getReading():
        if self.ser.is_open == False:
            self.ser.open()
        self.getTFminiData(self)

        if self.ser != None:
            self.ser.close()

    def getTFminiData(self):
        count = self.ser.in_waiting
        if count > 8:
            recv = self.ser.read(9)
            self.ser.reset_input_buffer()
            # type(recv), 'str' in python2(recv[0] = 'Y'), 'bytes' in python3(recv[0] = 89)
            # type(recv[0]), 'str' in python2, 'int' in python3

            if recv[0] == 0x59 and recv[1] == 0x59:     #python3
                distance = recv[2] + recv[3] * 256
                strength = recv[4] + recv[5] * 256
                print('(', distance, ',', strength, ')')
                self.ser.reset_input_buffer()


    def __del__(self):
        self.ser.close()
        #f.close()


lidarObj = lidarControl()
print(lidarObj.getReading());
