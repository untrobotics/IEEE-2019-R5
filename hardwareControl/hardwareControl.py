#This program accepts two int values representing the X and Y cordinate to move to as well as the rotation of the bot during the move

#First get data from C++ Code
import time
import math
import smbus

class hardwareControl:
    addr = 0x8 # bus address
    bus = smbus.SMBus(1) # indicates /dev/ic2-1

    currentX = 500
    currentY = 500
    currentRot = 0 #in degrees

    destX = 501
    destY = 500
    destRot = 0 #just moving forward to start with pi code

    V1 = 0
    V2 = 0
    V3 = 0
    V4 = 0

    grabber_Move_Flag = False
    grabber_Run_Flag  = False

    Vd = .5 #Desired Speed

    Angle = 0

    AngleSpeed = 0

    #Then reference the data to get distance/difference values
    diffX = destX - currentX
    diffY = destY - currentY
    diffRot = destRot - currentRot


    def send(self, inputString):
        self.bus.write_byte_data(self.addr, 0 , ord('<'))
        time.sleep(.001)
        for c in inputString:
            self.bus.write_byte_data(self.addr, 0, ord(c))
            time.sleep(.001)

        self.bus.write_byte_data(self.addr, 0 , ord('>'))
        time.sleep(.001)

    def move(self, x, y):
        #This is where the encoder calculation is going

        inputString = "drive,-30,-30,-30,-30,10000"
        self.send(inputString)

    def grabberMove(self, logic):
        global grabber_Move_Flag
        if(grabber_Move_Flag):
            inputString = "moveGrabber,2000,0"
            grabber_Move_Flag = True
        else:
            inputString = "moveGrabber,2000,1"
            grabber_Move_Flag = False
        self.send(inputString)


    def runGrabber(self, logic):
        global grabber_Run_Flag
        if(grabber_Run_Flag):
            inputString = "runGrabber,2000,{}".format(logic)
            print(inputString)
            grabber_Run_Flag = True
        else:
            inputString = "runGrabber,2000,1"
            grabber_Run_Flag = False
        self.send(inputString)


#Main instructions
#Here is where we will recieve our vector of commands
#vector from C++ Loaded here
controller = hardwareControl()

controller.move(1,2)
