from __future__ import division, print_function
import time
from hardwareControl2.tfmini_library import TFmini
from serial import *

# create the sensor and give it a port and (optional) operating mode


class lidarControl(object):
    tf = TFmini('/dev/ttyS0', mode=TFmini.STD_MODE)
    d = tf.read()
    # ser = serial.Serial("COM12", 115200)
    print('INFO: Lidar init done')

    # f=open("lidarDump.txt",a)

    def getReading(self):
        if self.d:
            return self.d[0]
        else:
            print('No valid response')

    def __del__(self):
        self.tf.close()
        # f.close()


lidarObj = lidarControl()
print(lidarObj.getReading());
