from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
import imutils

#from matplotlib import pyplot as plt
 
# initialize the camera and grab a reference to the raw camera capture
camera = PiCamera()
rawCapture = PiRGBArray(camera)
  
# allow the camera to warmup
time.sleep(0.2)
   
# grab an image from the camera
camera.capture(rawCapture, format="bgr")
image = rawCapture.array

# resize
scaleDown = 4
image = cv2.resize(image, (3280//scaleDown,2464//scaleDown))

# rotate
image = imutils.rotate(image,-90)

# convert to gray scale
#image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

#edges = cv2.Canny(image, 100, 200)

# display the image on screen and wait for a keypress
cv2.imshow("Image", image)
cv2.waitKey(0)













