build: HaptykConnect.c
	mkdir -p build/
	gcc HaptykConnect.c -Wall -g -c -I lib -fPIC -o build/HaptykConnect.o

link: build
	gcc build/HaptykConnect.o -L lib -lgattlib -o build/libhaptyk.o
	ar rcs libhaptyk.a build/libhaptyk.o

clean:
	rm -r build/
