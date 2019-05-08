debug: soundboard.cpp
	mkdir -p build/
	g++ -DDEBUG soundboard.cpp -Wall -g -c -o build/soundboard.o -I../haptyk/
	g++ build/soundboard.o -L../haptyk/ -lhaptyk -lgattlib -o soundboard.out

build: soundboard.cpp
	mkdir -p build/
	g++ soundboard.cpp -Wall -g -c -o build/soundboard.o -I../haptyk/
	g++ build/soundboard.o -L../haptyk/ -lhaptyk -lgattlib -o soundboard.out

run: build
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../haptyk/ ./soundboard.out CD:FB:24:E5:2F:14

valgrind: build
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../haptyk/ valgrind ./soundboard.out CD:FB:24:E5:2F:14

clean:
	rm -r build/
	rm ./soundboard.out
