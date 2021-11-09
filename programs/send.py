#
# Mimics key presses on keyboard to send data via putty
# Usage: python3 send.py path/to/file.ptf
#

import pyautogui
import time
import sys

print('1. Open your putty session')
print('2. Press any key - you\'ll have 3 seconds to click on putty terminal window')
input()

# wait until user clicks on putty window
time.sleep(3)
print('Sending bytes...')

try:
    # open file and mimic typing
    with open(sys.argv[1]) as f:
        for c in f.read():
            print(c, end='')
            pyautogui.press(c)

except:
    print('Usage: python3 send.py path/to/file.ptf');
