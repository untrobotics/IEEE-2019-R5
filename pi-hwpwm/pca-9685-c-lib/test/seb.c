#include "pca9685.h"

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>

#define PIN_BASE 300
#define MAX_PWM 4096
#define HERTZ 50

int main(void) {
	wiringPiSetup();

	int fd = pca9685Setup(PIN_BASE, 0x40, HERTZ);
	if (fd < 0)
	{
		printf("Error in setup\n");
		return fd;
	}

	// Reset all output
	pca9685PWMReset(fd);

	digitalWrite(PIN_BASE + 16, 1);
	//pwmWrite(PIN_BASE + 16, 0xfff);

	return 0;
}
