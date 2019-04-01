#!/usr/bin/python
import RPi.GPIO as GPIO
import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008
import time

CLK  = 18
MISO = 23
MOSI = 24
CS   = 25
mcp = Adafruit_MCP3008.MCP3008(clk=CLK, cs=CS, miso=MISO, mosi=MOSI)

try:
	PIN_TRIGGER = 26

	GPIO.setup(PIN_TRIGGER, GPIO.OUT)
	GPIO.output(PIN_TRIGGER, GPIO.LOW)

	print("Waiting for sensor to settle")
	time.sleep(2)
	print("Calculating distance")

	GPIO.output(PIN_TRIGGER, GPIO.HIGH)
	time.sleep(0.00001)
	GPIO.output(PIN_TRIGGER, GPIO.LOW)

	while mcp.read_adc(1) < 1000:
		pulse_start_time = time.time()

	pulse_started = 0
	while mcp.read_adc(1) > 1000 or pulse_started == 0:
		pulse_started = 1
		pulse_end_time = time.time()

	pulse_duration = pulse_end_time - pulse_start_time
	distance = round(pulse_duration * 17150, 2)
	print("Distance:", distance, "cm")

finally:
      GPIO.cleanup()
