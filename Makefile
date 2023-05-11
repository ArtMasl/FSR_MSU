all: lodepng.o paint_scull.o
	gcc -o analyzer.exe lodepng.o paint_scull.o -lm
lodepng.o: lodepng.h lodepng.c
	gcc -c lodepng.c
paint_scull.o: paint_scull.c
	gcc -c paint_scull.c
clean:
	rm *.o analyzer.exe
