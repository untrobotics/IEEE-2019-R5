from __future__ import division, print_function
import time
from tfmini_library import TFmini
from serial import *

# create the sensor and give it a port and (optional) operating mode
tf = TFmini('/dev/ttyS0', mode=TFmini.STD_MODE)

class lidarControl(object):
    d = tf.read()
    # ser = serial.Serial("COM12", 115200)
    print('INFO: Lidar init done')

    #f=open("lidarDump.txt",a)

    def getReading(self):
        if self.d:
            return self.d[0]
        else:
            print('No valid response')

        if self.ser != None:
            self.ser.close()


    # def getTFminiData(self):
    #     count = self.ser.in_waiting
    #     print(count)
    #     if count > 8:
    #         print('Test3')
    #         recv = self.ser.read(9)
    #         self.ser.reset_input_buffer()
    #         # type(recv), 'str' in python2(recv[0] = 'Y'), 'bytes' in python3(recv[0] = 89)
    #         # type(recv[0]), 'str' in python2, 'int' in python3
    #
    #         if recv[0] == 0x59 and recv[1] == 0x59:     #python3
    #             distance = recv[2] + recv[3] * 256
    #             strength = recv[4] + recv[5] * 256
    #             print('Test')
    #             print('(', distance, ',', strength, ')')
    #             self.ser.reset_input_buffer()


    def __del__(self):
        self.ser.close()
        #f.close()


lidarObj = lidarControl()
print(lidarObj.getReading());
