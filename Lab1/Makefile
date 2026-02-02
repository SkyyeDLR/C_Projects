parser: main.o build_tree.o
	gcc -o parser main.o build_tree.o

main.o: main.c build_tree.h
	gcc -I. -c main.c

build_tree.o: build_tree.c build_tree.h
	gcc -I. -c build_tree.c

clean:
	rm -f *.o parser