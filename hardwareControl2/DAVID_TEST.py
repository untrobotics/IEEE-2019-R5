import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(20,GPIO.OUT)
GPIO.setup(16, GPIO.IN, pull_up_down=GPIO.PUD_UP)

time.sleep(1)



print("SYSTEM READY")
GPIO.output(20, GPIO.HIGH)

while True:
    input_state = GPIO.input(16)
    if input_state == True:
        break

print("Starting mission...")
GPIO.output(20, GPIO.LOW)