# 
#	Haptyk SDK Makefile
#
#
CONFIG = ${pkg-config --cflags --libs glib-2.0} ${pkg-config --cflags --libs glib-2.0/include} $(pkg-config -r --cflags glib-2.0/glib)
CFLAGS =  -g -Wall -lbluetooth -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/glib-2.0/glib -lglib-2.0

all: HaptykConnect

HaptykConnect: HaptykConnect.o
	gcc ${CONFIG} ${CFLAGS}  -o HaptykConnect HaptykConnect.o

HaptykConnect.o: HaptykConnect.c
	gcc ${CONFIG} ${CFLAGS} -c HaptykConnect.c 

clean:
	rm -f HaptykConnect *.o *~
