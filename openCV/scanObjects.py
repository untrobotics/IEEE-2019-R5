from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
import imutils
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

# convert to gray scale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# resize
scaleDown = 4
gray = cv2.resize(gray, (3280//scaleDown, 2464//scaleDown))

# rotate
gray = imutils.rotate(gray, -90)

# initiate FAST object
# fast = cv2.FastFeatureDetector()

# corner detection
grayArr = np.float32(gray)

corners = cv2.goodFeaturesToTrack(grayArr, 1000, 0.01, 5)
corners = np.int0(corners)

for corner in corners:
    x, y = corner.ravel()
    cv2.circle(gray, (x,y), 3, 255, -1)


# img  = cv2.Canny(img, 100, 200)

# display the image on screen and wait for a keypress


cv2.imshow("Image", gray)
cv2.waitKey(0)













