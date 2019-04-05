from lidar import lidarControl
from hardwareControl import hardwareControl
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




lidarObj = lidar.lidarControl()

controller = file.hardwareControl(lidarObj)


print(lidarObj.getReading());

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
