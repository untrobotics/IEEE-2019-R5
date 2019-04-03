from board import SCL, SDA
import busio
import time


# Import the PCA9685 module.
from pca9685_library import PCA9685

# Create the I2C bus interface.
i2c_bus = busio.I2C(SCL, SDA)

# Create a simple PCA9685 class instance.
pca = PCA9685(i2c_bus)

# Set the PWM frequency to 60hz.
pca.frequency = 60

# Set the PWM duty cycle for channel zero to 50%. duty_cycle is 16 bits to match other PWM objects
# but the PCA9685 will only actually give 12 bits of resolution.

def cvt(percent):
    return round(65535*percent)

pca.channels[15].duty_cycle = 0
t_end = time.time() + 10

# stepper 1
while(time.time() < t_end):
    pca.channels[14].duty_cycle = cvt(0)   # Pwm 0-1
    pca.channels[14].duty_cycle = cvt(1)
    time.sleep(.0005)
    

pca.channels[14].duty_cycle = 0
pca.channels[15].duty_cycle = 0x0000

# stepper 2

# pca.channels[].duty_cycle = 0xffff







