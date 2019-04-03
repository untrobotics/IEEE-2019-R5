#This program accepts two int values representing the X and Y cordinate to move to as well as the rotation of the bot during the move

#First get data from C++ Code
import time
import math
import smbus

class hardwareControl:
    def __init__():
        self.addr = 0x8 # bus address
        self.bus = smbus.SMBus(1) # indicates /dev/ic2-1

        self.currentX = 500
        self.currentY = 500
        self.currentRot = 0 #in degrees

        self.destX = 501
        self.destY = 500
        self.destRot = 0 #just moving forward to start with pi code

        self.V1 = 0
        self.V2 = 0
        self.V3 = 0
        self.V4 = 0

        self.grabber_Move_Flag = False
        self.grabber_Run_Flag  = False

        self.Vd = .5 #Desired Speed

        self.Angle = 0

        self.AngleSpeed = 0

        #Then reference the data to get distance/difference values
        self.diffX = destX - currentX
        self.diffY = destY - currentY
        self.diffRot = destRot - currentRot


    def Send(inputString):
        bus.write_byte_data(addr, 0 , ord('<'))
        time.sleep(.001)
        for c in inputString:
            bus.write_byte_data(addr, 0, ord(c))
            time.sleep(.001)

        bus.write_byte_data(addr, 0 , ord('>'))
        time.sleep(.001)

    def move(x, y):
        #This is where the encoder calculation is going






        inputString = "drive,-30,-30,-30,-30,10000"
        Send(inputString)
    def grabberMove(logic):
        global grabber_Move_Flag
        if(grabber_Move_Flag):
            inputString = "moveGrabber,2000,0"
            grabber_Move_Flag = True
        else:
            inputString = "moveGrabber,2000,1"
            grabber_Move_Flag = False
        Send(inputString)


    def runGrabber(logic):
        global grabber_Run_Flag
        if(grabber_Run_Flag):
            inputString = "runGrabber,2000,{}".format(logic)
            print(inputString)
            grabber_Run_Flag = True
        else:
            inputString = "runGrabber,2000,1"
            grabber_Run_Flag = False
        Send(inputString)


#Main instructions
#Here is where we will recieve our vector of commands
#vector from C++ Loaded here
controller = hardwareControl()

move(1,2)
