from hardwareControl.lidar import lidarControl
from hardwareControl.hardwareControl import hardwareControl
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

# pathfinding

# nav to 1st block

# pickup block
