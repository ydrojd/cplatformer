out: main.o
	cc -o out main.o -lSDL2 -lSDL2_image

main.o: main.c
	cc -c main.c

clean:
	rm *.o out
