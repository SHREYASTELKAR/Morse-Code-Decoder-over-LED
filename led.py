import RPi.GPIO as GPIO
import time
import sys

# Define Morse code dictionary
morse_code = {
    'A': '.-', 'B': '-...', 'C': '-.-.', 'D': '-..', 'E': '.', 'F': '..-.', 'G': '--.', 'H': '....',
    'I': '..', 'J': '.---', 'K': '-.-', 'L': '.-..', 'M': '--', 'N': '-.', 'O': '---', 'P': '.--.',
    'Q': '--.-', 'R': '.-.', 'S': '...', 'T': '-', 'U': '..-', 'V': '...-', 'W': '.--', 'X': '-..-',
    'Y': '-.--', 'Z': '--..', '0': '-----', '1': '.----', '2': '..---', '3': '...--', '4': '....-',
    '5': '.....', '6': '-....', '7': '--...', '8': '---..', '9': '----.', '.': '.-.-.-', ',': '--..--',
    '?': '..--..', "'": '.----.', '!': '-.-.--', '/': '-..-.', '(': '-.--.', ')': '-.--.-', '&': '.-...',
    ':': '---...', ';': '-.-.-.', '=': '-...-', '+': '.-.-.', '-': '-....-', '_': '..--.-', '"': '.-..-.',
    '$': '...-..-', '@': '.--.-.', ' ': '/'
}

# Set up GPIO
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# LED pin
led_pin = 18
GPIO.setup(led_pin, GPIO.OUT)

def dot():
    GPIO.output(led_pin, GPIO.HIGH)
    time.sleep(0.019)
    GPIO.output(led_pin, GPIO.LOW)
    time.sleep(0.019)

def dash():
    #was 0.6, 1/2
    GPIO.output(led_pin, GPIO.HIGH)
    time.sleep(0.034)
    GPIO.output(led_pin, GPIO.LOW)
    time.sleep(0.019)

def transmit_morse_code(message):
    #was 0.2, 0.2, 0.4 0.2
    #0.08
    for char in message:
        char = char.upper()
        if char in morse_code:
            code = morse_code[char]
            for symbol in code:
                if symbol == '.':
                    dot()
                    time.sleep(0.055)   #sleep for dot and dash

                elif symbol == '-':
                    dash()
                    time.sleep(0.055)   #sleep for dot and dash
                else:  #encountered space
                    time.sleep(0.06)
            time.sleep(0.055)    #sleep for chars

        else:
            time.sleep(0.6)

# Example usage

"""
mess = input("./")
message = mess.split(' ', 2)
count = int(message[1])
"""

count = 1
args = sys.argv[1:]
message = ' '.join(args)

while count != 0:
    transmit_morse_code(message)
    #time.sleep(0.6)
    count -= 1
# Clean up GPIO
GPIO.cleanup()
