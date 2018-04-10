import RPi.GPIO as GPIO
import time

pin = 21

def get_time():
	return time.clock_gettime(time.CLOCK_MONOTONIC)

try:
	# use GPIO pin numbering
	GPIO.setmode(GPIO.BCM)

	GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

	active_last = False;
	time_last = get_time()
	while True:
		active = GPIO.input(pin)
		if active_last != active:
			active_last = active
			time_current = get_time()
			time_elapsed = time_current - time_last
			time_last = time_current
			state = 'on' if active else 'off'
			time_ms = time_elapsed * 1000.0
			if active:
				if time_ms < 20:
					pass # ignore noise
				elif time_ms < 150:
					print('.', end='', flush=True)
				elif time_ms < 400:
					print('-', end='', flush=True)
				else:
					print('long (' + str(time_ms) + 'ms', end='', flush=True)
			else:
				if time_ms < 300:
					pass # ignore short spaces
				elif time_ms < 1500:
					print(' ', end='', flush=True)
				else:
					print()
		time.sleep(0.005)

finally:
	GPIO.cleanup()
