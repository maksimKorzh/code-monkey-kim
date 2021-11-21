#
# Convert KIM-1 paper tape format to HEX dumo
#

# packages
import sys

# HEX dump
hexdump = ''

try:
    # get filename
    filename = sys.argv[1]

except Exception as e:
    print('Usage: python3 ppt2hex.py papertape.txt')
    sys.exit(1)

# open paper tape file
with open(filename) as f:
    # loop over paper tape lines
    for line in f.read().split('\n'):
        # extract bare bytes from line
        bytes = line[7:-4]
        
        # loop over bytes string
        for i in range(len(bytes)):
            # append byte to hex dump
            hexdump += bytes[i]
        
            # append space to hex dump
            if i % 2 != 0: hexdump += ' '

# output file name
outfile = filename.replace('.TXT', '.HEX')
print(outfile)

# write the output HEX dump
with open(outfile, 'w') as f:
    # loop over chars in HEX dump
    for char in hexdump:
        # write chat to file
        f.write(char)






