from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
import imutils
import math
import pytesseract

def distance(p0, p1):
    return math.sqrt((p0[0] - p1[0])**2 + (p0[1] - p1[1])**2)

# from matplotlib import pyplot as plt

#from matplotlib import pyplot as plt
 
# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
rawCapture = PiRGBArray(camera)
  
# allow the camera to warmup
time.sleep(0.2)
   
# grab an image from the camera
camera.capture(rawCapture, format="bgr")
img = rawCapture.array
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

# resize
scaleDown = 4
img2 = cv2.resize(img, (3280//scaleDown, 2464//scaleDown))

# rotate
img2 = imutils.rotate(img2, 180)

# convert to gray scale
gray = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)

# blur
#gray = cv2.GaussianBlur(gray, (5, 5), 0)

# contour
'''
ret, thresh = cv2.threshold(gray, 100, 255, cv2.THRESH_BINARY)

cv2.imshow("img", thresh)

cv2.waitKey()

im2, conts, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

cnt = conts[4]
cv2.drawContours(gray, [cnt], 0, (255,255,255), 4)
'''

gray = cv2.Canny(gray, 50, 60)

# corner detection
grayArr = np.float32(gray)

corners = cv2.goodFeaturesToTrack(grayArr, 30, 0.01, 10)
corners = np.int0(corners)


squareDistance = 150

for corner1 in corners:
    x1, y1 = corner1.ravel()
    
    '''
    for corner2 in corners:
        x2, y2 = corner2.ravel()
        #validSquare = true

        if (distance((x1,y1), (x1,y2)) < squareDistance):
            for corner3 in corners:
                x3, y3 = corner3.ravel()
                if (distance((x1,y1), (x3,y3)) < squareDistance and x3 != x2):
                    cv2.circle(gray, (x1,y1), 3, 0, -1)
                    cv2.circle(gray, (x2,y2), 3, 0, -1)
                    cv2.circle(gray, (x3,y3), 3, 0, -1)



        # PICKUP HERE
        # Shape detection, find squares, see pyimagesearch
        # vertical line check
        
        if (not (abs(x1 - x2) < 5 and abs(y1 - y2) > 70 and abs(y1-y2) < 100)):
            validSquare = false

        # horizontal line check
        if (not (abs(x1-x2) > 70 and abs(x1-x2) < 100 and abs(y1-y2) < 6)):
            validSquare = false

        # draw square
        if (validSquare):
            cv2.line(gray, (x1,y1), (x2,y2), (0,0,0), 2)
            #cv2.line(gray, (x
        
    '''
    #cv2.circle(gray, (x1,y1), 3, 255, -1)


# display the image on screen and wait for a keypress
cv2.imshow("img", img2)
print("Letters:" + pytesseract.image_to_string(img2))
cv2.line(gray, (500,500), (500, 600), (255,255,255), 2)
cv2.imshow("Image", gray)
cv2.waitKey(0)













