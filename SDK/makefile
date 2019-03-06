# 
#	Haptyk SDK Makefile
#
#
CFLAGS = -g -Wall

PKG_CONFIGS = $(pkg-config --cflags dbus-1)

INCLUDE_DIRS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
			   -I/usr/include/glib-2.0/glib -Ibluez/src -Ibluez/gdbus \
			   -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include

LIBRARIES = -lbluetooth -lglib-2.0

all: build

SRCS = HaptykConnect.c gatt.c att.c gattrib.c utils.c gatt-service.c gatttool.c \
	   uuid.c btio.c log.c

OBJS = $(patsubst %.c, %.o, $(SRCS))

build: $(SRCS)
	gcc $(PKG_CONFIGS) $(CFLAGS) $(INCLUDE_DIRS) -lglib-2.0 -c $(SRCS)
	ar rcs libhaptyk.a $(OBJS)

test: build
	gcc -c bt_test.c -o bt_test.o $(INCLUDE_DIRS)
	gcc -o test_haptyk bt_test.o -L. -lhaptyk -lglib-2.0 -lbluetooth

run: test
	./test_haptyk

clean:
	rm *.o test_haptyk
