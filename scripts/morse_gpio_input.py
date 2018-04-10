import RPi.GPIO as GPIO
import time

pin = 21

letters_to_morse = {
	'a': '.-',
	'b': '-...',
	'c': '-.-.',
	'd': '-..',
	'e': '.',
	'f': '..-.',
	'g': '--.',
	'h': '....',
	'i': '..',
	'j': '.---',
	'k': '-.-',
	'l': '.-..',
	'm': '--',
	'n': '-.',
	'o': '---',
	'p': '.--.',
	'q': '--.-',
	'r': '.-.',
	's': '...',
	't': '-',
	'u': '..-',
	'v': '...-',
	'w': '.--',
	'x': '-..-',
	'y': '-.--',
	'z': '--..'
}

morse_to_letters = {j: i for i, j in letters_to_morse.items()}

def get_time():
	return time.clock_gettime(time.CLOCK_MONOTONIC)

try:
	# use GPIO pin numbering
	GPIO.setmode(GPIO.BCM)

	GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

	active_last = False;
	time_last = get_time()
	letter = ''
	is_space = False
	while True:
		time_current = get_time()
		time_elapsed = time_current - time_last
		time_ms = time_elapsed * 1000.0
		if letter != '' and time_ms > 500:
			if letter in morse_to_letters:
				print(morse_to_letters[letter], end='', flush=True)
			else:
				print(letter, end='', flush=True)
			letter = ''
			is_space = False
		if not is_space and time_ms > 1500:
			print(' ', end='', flush=True)
			is_space = True
		active = GPIO.input(pin)
		if active_last != active:
			active_last = active
			time_last = time_current
			if active:
				if time_ms < 20:
					pass # ignore noise
				elif time_ms < 150:
					letter += '.'
					#print('.', end='', flush=True)
				else:
					letter += '-'
					#print('-', end='', flush=True)
				#else:
				#	print('long (' + str(time_ms) + 'ms', end='', flush=True)
		time.sleep(0.005)

finally:
	GPIO.cleanup()
