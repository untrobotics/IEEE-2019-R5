#This program accepts two int values representing the X and Y cordinate to move to as well as the rotation of the bot during the move

#First get data from C++ Code
import time
import math
import smbus
from lidar import lidarControl
from yaw import yaw


class hardwareControl:

    def __init__(self, lidarObj, yawObj):
        self.lidarObj = lidarObj
        self.yawObj = yawObj

    distance = 0

    addr = 0x08 # bus address
    bus = smbus.SMBus(1) # indicates /dev/ic2-1

    MoveFlag = 0 # Zero is the down state, 1 is the up state
    RunFlag  = 0

    currentX = 0
    currentY = 0
    currentRot = 0 #in degrees

    destX = 0
    destY = 0
    destRot = 0 #just moving forward to start with pi code

    V1 = 0
    V2 = 0
    V3 = 0
    V4 = 0

    encoderCorrection = 155 #correction value for the encoder

    moveGrabberUptime = 2200 #time to move grabber up and down consistently
    moveGrabberDowntime = 1900
    Vd = 100#Desired Speed

    Angle = 0



    AngleSpeed = 1

    #Then reference the data to get distance/difference values

    def readandMove(self):
        print("READ")
        f = open("path.txt",'r') #opening the file pointer to read from 
        #lines = f.readlines() #reads all of the lines from file

        for line in f: #gets each line in file 
            print("LOOP")

            commands = line.split(",")
            self.rotation(self, commands[2])
            time.sleep(.5)
            self.drive(self, commands[0], commands[1])
            time.sleep(.5)

    def send(self, inputString):
        timeout = 0
        while timeout < 15:
            try:
                self.bus.write_byte_data(self.addr, 0 , ord('<'))
                time.sleep(.001)
                for c in inputString:
                    self.bus.write_byte_data(self.addr, 0, ord(c))
                    time.sleep(.001)

                self.bus.write_byte_data(self.addr, 0 , ord('>'))
                time.sleep(.001)
            except:
                print("I/O Arduino Error, timeout = ", timeout)
                timeout += 1
                time.sleep(.01)
            finally:
                print("Message sent successfully")
                break

    def rotation(self, rot):
        self.destRot = float(rot) - self.currentRot
        if(self.destRot >0):
            self.V1 = 0
            self.V2 = 0
            self.V3 = 20
            self.V4 = 20
        
        #send the command with a distance do some trig or shit

    def rotate(self, turnAngle):
       

        startAngle = self.yawObj.getAngle()
        currentAngle = startAngle

        if turnAngle > 0:
            inputString = "drive,{},{},{},{},{}".format(-30, -30, 30, 30, 30000)
        else:
            inputString = "drive,{},{},{},{},{}".format(30, 30, -30, -30, 30000)

        self.send(inputString)
        c = 0
        while currentAngle < round(abs(turnAngle)-startAngle):
            lastAngle = currentAngle
            currentAngle = round(self.yawObj.getAngle()-startAngle)

            time.sleep(.01)

        print("INFO: Done rotating, current angle = ", currentAngle)
        inputString = "drive,{},{},{},{},{}".format(0, 0, 0, 0, 500)
        self.send(inputString)



        print("GYRO: {}".format(self.yawObj.getAngle()))

    
    def initScan(self):
        turnAngle = 360
        # rotate negative angles not working, debug
        f = open("lidar2.txt", "w")

        startAngle = self.yawObj.getAngle()
        
        currentAngle = startAngle

        inputString = "drive,{},{},{},{},{}".format(30, 30, -30, -30, 30000)

        self.send(inputString)
        c = 0
        while currentAngle < round(abs(turnAngle) - startAngle):
            lastAngle = currentAngle

            currentAngle = round((self.yawObj.getAngle() - startAngle), 2)
            
            if (currentAngle % .25 > 0 and currentAngle % .25 < .2):
                print(currentAngle, ",", self.lidarObj.getReading())
                writeString = (str(currentAngle) + "," + str(self.lidarObj.getReading()) + "\n")
                f.write(writeString)

            time.sleep(.01)

            # if (c % 10000 == 0):
            #     print(currentAngle, " < ", round(turnAngle-startAngle))
            # if (c == 200000):
            #     c = 0
            # c += 1

        print("INFO: Done rotating, current angle = ", currentAngle)
        inputString = "drive,{},{},{},{},{}".format(0, 0, 0, 0, 500)
        self.send(inputString)

        f.close()

        print("GYRO: {}".format(self.yawObj.getAngle()))



    def drive(self, x, y):
        #print("Driving...")

        global distance

        global encoderCorrection 

        #print("X: "+x))

       # if(float(x) > 0):
        #     print("LEFT?")
        #     self.V1 = 80
        #     self.V2 = -80
        #     self.V3 = -140
        #     self.V4 = 140
        #     self.distance = (abs(float(x))*self.encoderCorrection)
        # elif(float(x) < 0):
        #     print("Right?")
        #     self.V1 = -80
        #     self.V2 = 80
        #     self.V3 = 140
        #     self.V4 = -130 
        #     self.distance = (abs(float(x))*self.encoderCorrection)
        if(float(y) > 0):
            print("forward")
            self.V1 = 35
            self.V2 = 35
            self.V3 = 30
            self.V4 = 30
            self.distance = (abs(float(y))*self.encoderCorrection)
        elif(float(y) < 0):
            print("backward")
            self.V1 = -30
            self.V2 = -30
            self.V3 = -35
            self.V4 = -35 
            self.distance = (abs(float(y))*self.encoderCorrection)


        print("V1: {} V2: {} V3: {} V4: {}".format(self.V1,self.V2,self.V3,self.V4))
        print ("Distance: {}".format(self.distance))
        print("drive,{},{},{},{},{}".format(round(self.V1, 3),round(self.V2,3),round(self.V3,3),round(self.V4,3),int(round(self.distance,3))))

        inputString = "drive,{},{},{},{},{}".format(round(self.V1, 3),round(self.V2,3),round(self.V3,3),round(self.V4,3),int(round(self.distance,3)))
        self.send(inputString)

        self.currentX = float(x)
        self.currentY = float(y)

    def moveGrabber(self, logic):
        global MoveFlag
        global moveGrabberUptime
        global moveGrabberDowntime  
        if(logic == 1): #Moves the grabber up
            if(self.MoveFlag == 0):
                inputString = "moveGrabber,{},1".format(self.moveGrabberUptime)
                self.MoveFlag = 1
                self.send(inputString)
            else:
               print ("Don't do that shit")
        elif (logic == 0):
            print("RUNNING")
            if(self.MoveFlag == 1):
                inputString = "moveGrabber,{},0".format(self.moveGrabberDowntime) #move the grabber down if 1
                self.MoveFlag = 0
                self.send(inputString)
            else:
                print ("Do it right and dont fuck it up")



    def runGrabber(self, logic):


        inputString = "runGrabber,1500,{}".format(logic)

        self.send(inputString)




    def pickupBlock(self):

        #this function is a predefined method to pick up a block. Should work as long as we are over 30 cm away
        distance = 0
        distanceAway = self.lidarObj.getReading() #get the actual distance we are away

        print(distanceAway)

        if(distanceAway >= .34):
            self.drive(0,(distanceAway*100)) 
        else:
            self.drive(0,0)

        time.sleep(5)
        self.runGrabber(0)  #block should be picked up here    
        time.sleep(1) #waiting 1 


        #get lidar distance

        # distanceToDrive = distanceAway - 10

        # self.drive(0,(distanceAway - distanceToDrive) ) #move the bot a certain amount towards the block12) #move the bot a certain amount towards the block
        # # translate left until block is out of view
        # while( self.lidarObj.getReading() > .5 ):
        #     self.drive(-.5,0) #drive the bot left until we read a far distance
        #     time.sleep(.5)

        # while(self.lidarObj.getReading() > .5 ):
        #     self.drive(.5,0)
        #     distance += .5
        #     time.sleep(.5)

        # # translate right from block detected to half block
        # for x in (0,distance/2, .5): #goes back half
        #     self.drive(-.5,0) #drive back half the distance
        #     time.sleep(.5)

        # # drive forward X distance
        # self.drive(0,(distanceToDrive-1)) #move the rest of the distance towards the block minus 1 centimeter

        # # move up grabber
        # self.moveGrabber( 1) #1 should be up and will move grabber up
        # # drive forward a little more
        # self.drive(0,1) #drive forward 1 more centemeter to be centered on the wheel
        # # lower grabber and turn on grabber
        # while(self.lidarObj.getReading() > .5): #wile we are reading something in front loop these controls
        #     runGrabber( 1) #start the grabber wheel
        #     moveGrabber(0) #move the grabber down
        #     time.sleep(2) #give it time to pick up
        #     moveGrabber(1) #move the grabber up to see if we got the block
        # # check if block picked up
        

#Main instructions
#Here is where we will recieve our vector of commands
#vector from C++ Loaded here

#test = hardwareControl

#test.readandMove(test) #getting data from file

#test.pickupBlock(test)


