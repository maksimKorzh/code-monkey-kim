all: wozaciF100.mon

wozaciF100.mon: wozaciF100.bin
	bintomon -v `./getaddress` wozaciF100.bin >wozaciF100.mon

wozaciF100.bin: wozaciF100.o
	ld65 -t none -vm -m wozaciF100.map -o wozaciF100.bin wozaciF100.o

wozaciF100.o:	wozaciF100.s
	ca65 -g -l wozaciF100.lst wozaciF100.s

clean:
	$(RM) *.o *.lst *.map *.bin

distclean: clean
	$(RM) *.mon
