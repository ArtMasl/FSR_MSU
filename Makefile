all: lodepng.o 1course.o
	gcc -o analyzer.exe lodepng.o 1course.o
lodepng.o: lodepng.h lodepng.c
	gcc -c lodepng.c
1course.o: 1course.c
	gcc -lm -c 1course.c
