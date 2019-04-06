from lidar import lidarControl
from hardwareControl import hardwareControl
from yaw import yaw

import threading
import time
import math

ROUND = 1

if (ROUND == 1):
    NUMOFBLOCKS = 2
    NUMOFOBS = 5
elif (ROUND == 2):
    NUMOFBLOCKS = 4
    NUMOFOBS = 10
else:
    NUMOFBLOCKS = 6
    NUMOFOBS = 15

yawObj = yaw()
lidarObj = lidarControl()
controller = hardwareControl(lidarObj, yawObj)

def backgroundLoop():
    while 1:
        yawObj.loop()
        time.sleep(.01)


def foreground():
    #print("LIDAR: ", lidarObj.getReading())
    #controller.rotate(360)
    
    #controller.drive(0,20)
    #time.sleep(6)
    #controller.moveGrabber(1)
    #time.sleep(3)
    #controller.drive(0,5)
    #time.sleep(2)
    #controller.moveGrabber(0)
    #time.sleep(2.2)
    #controller.runGrabber(0)
    #time.sleep(2.2)
    #controller.runGrabber(1)

    controller.initScan()

    #controller.drive(0,0)
    #controller.pickupBlock()
    #controller.rotation(360)

b = threading.Thread(name='background', target=backgroundLoop)
f = threading.Thread(name='foreground', target=foreground)

b.start()
f.start()

#controller.move(1,2)


# 360 scan
    # detect objects
    # MAYBE: openCV

# for each block
    # make path

    # nav to 1st block

    # pickup block procedure

# make path to mothership

# drop blocks off
    # line up with first bin

    # foreach block
        #line up bin

        # drop block

        # move left

# RTH
