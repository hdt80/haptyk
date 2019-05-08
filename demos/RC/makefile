debug: rc.c
	mkdir -p build/
	gcc -DDEBUG rc.c -Wall -g -c -o build/rc.o -I../haptyk/
	gcc build/rc.o -L../haptyk/ -lhaptyk -lgattlib -o rc.out

build: rc.c
	mkdir -p build/
	gcc rc.c -Wall -g -c -o build/rc.o -I../haptyk/
	gcc build/rc.o -L../haptyk/ -lhaptyk -lgattlib -o rc.out

run: build
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../haptyk/ ./rc.out CD:FB:24:E5:2F:14

valgrind: build
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../haptyk/ valgrind ./rc.out CD:FB:24:E5:2F:14

clean:
	rm -r build/
	rm ./rc.out
