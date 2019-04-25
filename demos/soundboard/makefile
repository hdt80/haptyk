debug: soundboard.c
	mkdir -p build/
	gcc -DDEBUG soundboard.c -Wall -g -c -o build/soundboard.o -I../haptyk/
	gcc build/soundboard.o -L../haptyk/ -lhaptyk -lgattlib -o soundboard.out

build: soundboard.c
	mkdir -p build/
	gcc soundboard.c -Wall -g -c -o build/soundboard.o -I../haptyk/
	gcc build/soundboard.o -L../haptyk/ -lhaptyk -lgattlib -o soundboard.out

run: build
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../haptyk/ ./soundboard.out CD:FB:24:E5:2F:14

clean:
	rm -r build/
	rm ./soundboard.out
