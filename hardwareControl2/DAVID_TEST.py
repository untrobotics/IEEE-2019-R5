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


        if (currentAngle % .25 == 0):
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