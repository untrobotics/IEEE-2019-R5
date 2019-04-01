from board import SCL, SDA
import busio
import time

# Import the PCA9685 module.
from pca9685_library import PCA9685

i2c_bus = busio.I2C(SCL, SDA)
pca = PCA9685(i2c_bus)
pca.frequency = 50

M1_IN1 = 2
M1_IN2 = 3
M1_ENA = 0

M1_IN3 = 4
M1_IN4 = 5
M1_ENB = 1

pca.channels[M1_IN1].duty_cycle = 0x0000
pca.channels[M1_IN2].duty_cycle = 0x0000

time.sleep(0.5)

# Set the PWM duty cycle for channel zero to 50%. duty_cycle is 16 bits to match other PWM objects
# but the PCA9685 will only actually give 12 bits of resolution.
pca.channels[M1_IN1].duty_cycle = 0xffff
pca.channels[M1_IN2].duty_cycle = 0x0000
pca.channels[M1_ENA].duty_cycle = 0x7fff
