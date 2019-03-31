# Simple example of reading the MCP3008 analog input channels and printing
# them all out.
# Author: Tony DiCola
# License: Public Domain
import time

# Import SPI library (for hardware SPI) and MCP3008 library.
import Adafruit_GPIO.SPI as SPI
import Adafruit_MCP3008

CLK  = 18
MISO = 23
MOSI = 24
CS   = 25
mcp = Adafruit_MCP3008.MCP3008(clk=CLK, cs=CS, miso=MISO, mosi=MOSI)

while True:
	print("=============================");
	print("Channel 0", mcp.read_adc(0));
	print("Channel 1", mcp.read_adc(1));
	print("Channel 2", mcp.read_adc(2));
	print("Channel 3", mcp.read_adc(3));
	print("Channel 4", mcp.read_adc(4));
	print("Channel 5", mcp.read_adc(5));
	print("Channel 6", mcp.read_adc(6));
	print("Channel 7", mcp.read_adc(7));
	time.sleep(0.1)
