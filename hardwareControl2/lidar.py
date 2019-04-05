from __future__ import division, print_function
import time
from tfmini_library import TFmini

# create the sensor and give it a port and (optional) operating mode


class lidarControl(object):
    # 0.34
    tf = TFmini('/dev/ttyS0', mode=TFmini.STD_MODE)

    # ser = serial.Serial("COM12", 115200)
    print('INFO: Lidar init done')

    # f=open("lidarDump.txt",a)

    def getReading(self):
        d = self.tf.read()
        if d:
            return d[0]
        else:
            print('No valid response')

    def __del__(self):
        self.tf.close()
        # f.close()
