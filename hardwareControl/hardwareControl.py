#This program accepts two int values representing the X and Y cordinate to move to as well as the rotation of the bot during the move

#First get data from C++ Code
import time
import math
import smbus


class hardwareControl:

    distance = 0

    addr = 0x8 # bus address
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

<<<<<<< HEAD
    def readandMove(self):
        print("READ")
        f = open("path.txt",'r') #opening the file pointer to read from 
        #lines = f.readlines() #reads all of the lines from file

        for line in f: #gets each line in file 
            print("LOOP")
=======
    def readData(self):
        f = open("path.txt",'r') #opening the file pointer to read from
        lines = f.readlines() #reads all of the lines from file

        for i, line in enumerate(lines): #gets each line in file
>>>>>>> 0ce8e7a41f52bcfd3f777866db1da356ad412f0d
            commands = line.split(",")
            self.rotation(self, commands[2])
            time.sleep(.5)
            self.drive(self, commands[0], commands[1])
            time.sleep(.5)

    def send(self, inputString):
        self.bus.write_byte_data(self.addr, 0 , ord('<'))
        time.sleep(.001)
        for c in inputString:
            self.bus.write_byte_data(self.addr, 0, ord(c))
            time.sleep(.001)

        self.bus.write_byte_data(self.addr, 0 , ord('>'))
        time.sleep(.001)

    def rotation(self, rot):
        self.destRot = float(rot) - self.currentRot
        if(self.destRot >0):
            self.V1 = 0
            self.V2 = 0
            self.V3 = 20
            self.V4 = 20
        
        #send the command with a distance do some trig or shit


    def drive(self, x, y):
        global distance
<<<<<<< HEAD
        global encoderCorrection 
        self.distance = math.sqrt((pow((float(x)-self.currentX),2)) + (pow((float(y)-self.currentY),2))) #this should give us a distance to travel
        self.distance *= self.encoderCorrection #change the encoder value 
        #print("X: "+x))
        if(float(x) != 0.0):


            self.Angle = math.radians(math.atan(float(y)/float(x)))#i think this is right 

        else:
            if(float(y) > 0):
                self.Angle = (math.pi/2)
            else:
                self.Angle = (3 * math.pi)/2

        
        print((self.Angle))


=======
        global encoderCorrection
        self.distance = math.sqrt((pow((x-self.currentX),2)) + (pow((y-self.currentY),2))) #this should give us a distance to travel
        self.distance *= self.encoderCorrection #change the encoder value

#set v value

        self.V1 = 30
        self.V2 = 30
        self.V3 = 30
        self.V4 = 30
>>>>>>> 0ce8e7a41f52bcfd3f777866db1da356ad412f0d

       # self.V1 = self.Vd*(math.cos(self.Angle - (math.pi/4)))
        #self.V2 = self.Vd*(math.sin(self.Angle - (math.pi/4)))
        #self.V3 = self.Vd*(math.sin(self.Angle - (math.pi/4)))
        #self.V4 = self.Vd*(math.cos(self.Angle - (math.pi/4)))

        
        self.V1 = 90
        self.V2 = -90
        self.V3 = -90
        self.V4 = 90    #print ("X: {} and Y: {}".format(x,y))
        print("V1: {} V2: {} V3: {} V4: {}".format(self.V1,self.V2,self.V3,self.V4))
        print ("Distance: {}".format(self.distance))
        print("drive,{},{},{},{},{}".format(round(self.V1, 3),round(self.V2,3),round(self.V3,3),round(self.V4,3),int(round(self.distance,3))))

        inputString = "drive,{},{},{},{},{}".format(round(self.V1, 3),round(self.V2,3),round(self.V3,3),round(self.V4,3),int(round(self.distance,3)))
        self.send(self,inputString)

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
                self.send(self,inputString)
            else:
               print ("Don't do that shit")
        elif (logic == 0):
            if(self.MoveFlag == 1):
                inputString = "moveGrabber,{},0".format(self.moveGrabberDowntime) #move the grabber down if 1
                self.MoveFlag = 0
                self.send(self,inputString)
            else:
                print ("Do it right and dont fuck it up")



    def runGrabber(self, logic):


        inputString = "runGrabber,1500,{}".format(logic)

        self.send(self,inputString)




    def pickupBlock(self):
        
        #self.distance = 2000
        # get flush
        # drive X distance
        #get lidar distance
        #self.drive(self,0,12) #move the bot a certain amount towards the block
        # translate left until block is out of view
       # while(#return Value < self.lidar.read() || ):
        #self.drive(self,-.5,0) #drive the bot left until we read a far distance
       # time.sleep(.5)

       # while(#return value  == self.lidar.read()):
        #self.drive(self,.5,0)
        #self.distance += .5
        #time.sleep(.5)
        #Sself.drive(self,1,1)
        #self.runGrabber(self,1)
        #time.sleep(4)
        #self.drive(self,0,20)
        #time.sleep(.5)
            #time.sleep(2.1)
            #self.moveGrabber(self,1)
            #time.sleep(3)
            #self.moveGrabber(self,0)

        self.drive(self,30,30)
        time.sleep(2.5)
        # translate right from block detected to half block
        #for x in (0,self.distance/2, .5): #goes back half
         #   self.drive(self,-.5, 0) #drive back half the distance
         #   time.sleep(.5)

        # drive forward X distance
        #get encoder data
        #drive(self,0,(self.lidar.read()-1)) #move the rest of the distance towards the block minus 1 centimeter

        # move up grabber
        #self.moveGrabber(self, 1) #1 should be up and will move grabber up
        # drive forward a little more
        #self.drive(self,0,1) #drive forward 1 more centemeter to be centered on the wheel
        # lower grabber and turn on grabber
        #while(self.lidar.read() <= 5): #wile we are reading something in front loop these controls
         #   runGrabber(self, 1) #start the grabber wheel
          #  moveGrabber(self,0) #move the grabber down
           # time.sleep(2) #give it time to pick up
           # moveGrabber(self,1) #move the grabber up to see if we got the block
        # check if block picked up


#Main instructions
#Here is where we will recieve our vector of commands
#vector from C++ Loaded here

test = hardwareControl
<<<<<<< HEAD
#test.readandMove(test) #getting data from file

test.pickupBlock(test)
=======
test.readData(test)
test.pickupBlock(test)
>>>>>>> 0ce8e7a41f52bcfd3f777866db1da356ad412f0d
