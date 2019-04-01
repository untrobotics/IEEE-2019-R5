# pip3 install rpimotorlib

import time
import RPi.GPIO as GPIO

# import the library
from RpiMotorLib import RpiMotorLib

GpioPins = [11, 9, 5, 6]

# Declare an named instance of class pass a name and type of motor
mymotortest = RpiMotorLib.BYJMotor("TRANSLATE_MOTOR", "Nema")
time.sleep(0.5)

# call the function pass the parameters
mymotortest.motor_run(GpioPins , 0.1, 10, False, True, "half", .05)

# good practise to cleanup GPIO at some point before exit
GPIO.cleanup()
