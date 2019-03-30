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

# gray
gray = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)

template = cv2.imread('B2.png', 0)
w, h = template.shape[::-1]

res = cv2.matchTemplate(gray, template, cv2.TM_CCOEFF_NORMED)
threshold = 0.8
loc = np.where(res >= threshold)

for point in zip(*loc[::-1]):
    print('Found')
    cv2.rectangle(img2, point, (point[0]+w, point[1]+h), (0,255,255), 2)

cv2.imshow('detected', img2)
cv2.waitKey(0)

