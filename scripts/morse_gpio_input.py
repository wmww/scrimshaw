import RPi.GPIO as GPIO
from time import sleep

pin = 21

try:
	# use GPIO pin numbering
	GPIO.setmode(GPIO.BCM)

	GPIO.setup(pin, GPIO.IN)

	for i in range(50):
		if GPIO.input(pin):
			print('on')
		else:
			print('off')
		sleep(0.1)

finally:
	GPIO.cleanup()
