from board import SCL, SDA
import busio

# Import the PCA9685 module.
from pca9685_library import PCA9685

i2c_bus = busio.I2C(SCL, SDA)
pca = PCA9685(i2c_bus)
pca.frequency = 60

# Set the PWM duty cycle for channel zero to 50%. duty_cycle is 16 bits to match other PWM objects
# but the PCA9685 will only actually give 12 bits of resolution.
pca.channels[0].duty_cycle = 0x0000
pca.channels[1].duty_cycle = 0x0000
pca.channels[2].duty_cycle = 0x0000
pca.channels[3].duty_cycle = 0x0000
pca.channels[4].duty_cycle = 0x0000
pca.channels[5].duty_cycle = 0x0000
pca.channels[6].duty_cycle = 0x0000
pca.channels[7].duty_cycle = 0x0000
pca.channels[8].duty_cycle = 0x0000
pca.channels[9].duty_cycle = 0x0000
pca.channels[10].duty_cycle = 0x0000
pca.channels[11].duty_cycle = 0x0000
pca.channels[12].duty_cycle = 0x0000
pca.channels[13].duty_cycle = 0x0000
pca.channels[14].duty_cycle = 0x0000
pca.channels[15].duty_cycle = 0x0000
