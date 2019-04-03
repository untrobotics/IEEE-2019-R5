from __future__ import division, print_function
import time
from tfmini import TFmini

# create the sensor and give it a port and (optional) operating mode


class lidarControl:
    def __init__(self):
        tf = TFmini('/dev/ttyS0', mode=TFmini.STD_MODE)
        print('INFO: Lidar init done');

    #f=open("lidarDump.txt",a)

    def getReading(self):
        d = self.tf.read()
        if d:
            #f.write('Distance: {:5}, Strength: {:5}'.format(d[0],d[1]))
            return d[0]
            #print('Distance: {:5}, Strength: {:5}'.format(d[0], d[1]))
        else:
            print('INFO: Lidar no valid response')


    def __del__(self):
        self.tf.close()
        #f.close()


lidarObj = lidar.lidarControl()
print(lidarObj.getReading());
