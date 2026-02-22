compile: main.o build_tree.o backend.o
	gcc -o compile main.o build_tree.o backend.o

main.o: main.c build_tree.h
	gcc -I. -c main.c

build_tree.o: build_tree.c build_tree.h
	gcc -I. -c build_tree.c

backend.o: backend.c
	gcc -I. -c backend.c

clean:
	rm -f *.o compile