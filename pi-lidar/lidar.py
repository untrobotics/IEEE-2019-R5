from __future__ import division, print_function
import time
from tfmini_library import TFmini

# create the sensor and give it a port and (optional) operating mode
tf = TFmini('/dev/ttyS0', mode=TFmini.STD_MODE)

#f=open("lidarDump.txt",a)

print('init done');

try:
    print('='*25)
    while True:
        d = tf.read()
        if d:
            #f.write('Distance: {:5}, Strength: {:5}'.format(d[0],d[1]))
            print('Distance: {:5}, Strength: {:5}'.format(d[0], d[1]))
        else:
            print('No valid response')
        time.sleep(0.1)

except KeyboardInterrupt:
    tf.close()
    f.close()
    print('bye!!')
