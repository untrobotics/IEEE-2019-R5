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
camera.awb_mode = 'auto'
camera.capture(rawCapture, format='bgr')
image = rawCapture.array

image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

# resize
scaleDown = 4
image = cv2.resize(image, (3280//scaleDown,2464//scaleDown))

# rotate
image = imutils.rotate(image,180)

hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

#image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

lowerWhite = np.array([70,0,50])
upperWhite = np.array([180,180,180])

mask = cv2.inRange(hsv, lowerWhite, upperWhite)
res = cv2.bitwise_and(image, image, mask = mask)


# display the image on screen and wait for a keypress
cv2.imshow("Image", image)
cv2.imshow("mask", mask)
cv2.imshow("res", res)
cv2.imwrite('test.png', image)
k = cv2.waitKey(0)

cv2.destroyallWindows()
cap.release()











