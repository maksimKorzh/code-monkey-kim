#
# Uploads hexdump to https://www.asm80.com/kim.html
#      via imitating keypresses on keyboard
#

# packages
import pyautogui
import time
import sys

# warning message
print('Adjust address in the emulator,')
print('make sure you\'re in the DATA mode.')
print('Press any key... you\'ll have 3 seconds to click on emulator window')

# waiting for any key to be pressed
input()

# wait until user clicks the emulator window
time.sleep(3)

try:
    # open target file to upload
    with open(sys.argv[1]) as f:
        # loop over the characters
        for char in f.read().replace('\n', ' '):
            pyautogui.keyDown(char)
            print(char)
            pyautogui.keyUp(char)
except:
    print('Usage: python3 type.py file.hex')
