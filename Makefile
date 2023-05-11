all: lodepng.o paint_rentgen.o
	gcc -o analyzer.exe lodepng.o paint_rentgen.o -lm
lodepng.o: lodepng.h lodepng.c
	gcc -c lodepng.c
paint_rentgen.o: paint_rentgen.c
	gcc -c paint_rentgen.c
clean:
	rm *.o analyzer.exe
