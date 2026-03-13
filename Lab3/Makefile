disassemble: main.o disassemble.o
	gcc -I. -o disassemble main.o disassemble.o

main.o: main.c disassemble.h
	gcc -I. -c main.c

disassemble.o: disassemble.c disassemble.h
	gcc -I. -c disassemble.c

clean:
	rm -f *.o disassemble