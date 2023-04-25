all: lodepng.o testSobel.o
	gcc -o sobel.exe lodepng.o testSobel.o -lm
lodepng.o: lodepng.h lodepng.c
	gcc -c lodepng.c
testSobel.o: testSobel.c
	gcc -c testSobel.c
clean:
	rm *.o sobel.exe
