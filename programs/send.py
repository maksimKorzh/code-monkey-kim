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

# paper tape
paper = ''

try:
    # open file and mimic typing
    with open(sys.argv[1]) as f:
        paper = f.read()

except:
    print('Usage: python3 send.py path/to/file.ptf');

# send butes
print('Sending bytes...')
for c in paper:
    print(c, end='')
    pyautogui.press(c)
    if c == '\n': pyautogui.press('\r')
