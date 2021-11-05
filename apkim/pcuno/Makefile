CC=gcc
CFLAGS= -lcdk -lncurses -I /usr/include/cdk

kim: pcuno.c ../kimuno/cpu.c dcc6502.c fs.c ../kimuno/roms.h ../kimuno/config.h ../kimuno/kimuno.h
	$(CC) -g -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o ../kimuno/*.o kim

all: kim
