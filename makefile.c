all: main.o
	gcc main.o -o main -lm
	
main.o: Холмяк.c stb_image.h
	 gcc -c Холмяк.c