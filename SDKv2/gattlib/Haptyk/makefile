build: HaptykConnect.c
	mkdir -p build/
	gcc HaptykConnect.c -Wall -g -c -I lib -fPIC -o build/HaptykConnect.o

link: build
	#gcc build/HaptykConnect.o -g -c -L lib -lgattlib -fPIC -o build/libhaptyk.o
	ar rcs libhaptyk.a build/HaptykConnect.o

clean:
	rm -r build/
	rm libhaptyk.a

test: link
	gcc HaptykTest.c -Wall -g -c -o HaptykTest.o -I lib
	gcc HaptykTest.o -L . -lhaptyk -L lib -lgattlib -o build/test.out
	cp lib/libgattlib.so build/

run: test
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/ ./build/test.out

debug: test
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/ gdb ./build/test.out
