all: miniFBAE.mon

miniFBAE.mon: miniFBAE.bin
	bintomon -v `./getaddress` miniFBAE.bin >miniFBAE.mon

miniFBAE.bin: miniFBAE.o
	ld65 -t none -vm -m miniFBAE.map -o miniFBAE.bin miniFBAE.o

miniFBAE.o:	miniFBAE.s
	ca65 -g -l miniFBAE.lst miniFBAE.s

clean:
	$(RM) *.o *.lst *.map *.bin

distclean: clean
	$(RM) *.mon
