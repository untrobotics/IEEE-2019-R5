#!/usr/bin/python

'''
To install:
raspi-config (enable I2C)

Add:
i2c-bcm2708
i2c-dev

to /etc/modules

sudo apt-get install i2c-tools python-smbus
i2cdetect -y 1 (should show a device with 0x68)
'''

import smbus
import math
import time

#

#print "gyroskop_xout: ", ("%5d" % gyroskop_xout), " skaliert: ", (gyroskop_xout / 131) # roll
#print "gyroskop_yout: ", ("%5d" % gyroskop_yout), " skaliert: ", (gyroskop_yout / 131) # pitch



class yaw(object):
    angle = 0;
    version = 0;

    power_mgmt_1 = 0x6b
    power_mgmt_2 = 0x6c

    bus = smbus.SMBus(1)  # bus = smbus.SMBus(0) fuer Revision 1
    address = 0x68

    # Aktivieren, um das Modul ansprechen zu koennen
    bus.write_byte_data(address, power_mgmt_1, 0)

    # gyroskop_xout = lesen_wort_2c(0x43)
    # gyroskop_yout = lesen_wort_2c(0x45)
    # gyroskop_zout = lesen_wort_2c(0x47)

    def lesen_byte(self,reg):
        return self.bus.read_byte_data(self.address, reg)

    def lesen_wort(self,reg):
        h = self.bus.read_byte_data(self.address, reg)
        l = self.bus.read_byte_data(self.address, reg+1)
        value = (h << 8) + l
        return value

    def lesen_wort_2c(self,reg):
        val = self.lesen_wort(reg)
        if (val >= 0x8000):
            return -((65535 - val) + 1)
        else:
            return val

    def dist(self,a,b):
        return math.sqrt((a*a)+(b*b))

    def get_y_rotation(self,x,y,z):
        radians = math.atan2(x, self.dist(y,z))
        return -math.degrees(radians)

    def get_x_rotation(self,x,y,z):
        radians = math.atan2(y, self.dist(x,z))
        return math.degrees(radians)

    c = 0
    def loop(self):
        # make the while loop less intensive, increase 200000 to slow down
        self.c += 1
        if self.c == 20000:
            self.c = 0

        if self.c % 10 == 0:
            pitch_raw = self.lesen_wort_2c(0x45)
            pitch = pitch_raw / 131
            # print("PITCH", pitch)

            # smooth signal
            if abs(pitch) > 2:
                self.angle = self.angle + pitch

    # roll_raw = lesen_wort_2c(0x43)
    # roll = roll_raw / 131
    # print("ROLL", roll)

    # change 300000 to adjust angle, higher less spin
    def getAngle(self):
        return -round(self.angle/320000*360)

    # print("ANGLE: ", angle)

    # yaw_raw = lesen_wort_2c(0x47)
    # yaw = yaw_raw / 131
    # print("YAW", yaw);

    # yaw is backwards

#print "gyroskop_zout: ", ("%5d" % gyroskop_zout), " skaliert: ", (gyroskop_zout / 131) # yaw

#print
#print "Beschleunigungssensor"
#print "---------------------"

#beschleunigung_xout = lesen_wort_2c(0x3b)
#beschleunigung_yout = lesen_wort_2c(0x3d)
#beschleunigung_zout = lesen_wort_2c(0x3f)

#beschleunigung_xout_skaliert = beschleunigung_xout / 16384.0
#beschleunigung_yout_skaliert = beschleunigung_yout / 16384.0
#beschleunigung_zout_skaliert = beschleunigung_zout / 16384.0

#print "beschleunigung_xout: ", ("%6d" % beschleunigung_xout), " skaliert: ", beschleunigung_xout_skaliert
#print "beschleunigung_yout: ", ("%6d" % beschleunigung_yout), " skaliert: ", beschleunigung_yout_skaliert
#print "beschleunigung_zout: ", ("%6d" % beschleunigung_zout), " skaliert: ", beschleunigung_zout_skaliert

#print "X Rotation: " , get_x_rotation(beschleunigung_xout_skaliert, beschleunigung_yout_skaliert, beschleunigung_zout_skaliert)
#print "Y Rotation: " , get_y_rotation(beschleunigung_xout_skaliert, beschleunigung_yout_skaliert, beschleunigung_zout_skaliert)
