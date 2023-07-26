CFLAGS = -Wall -std=c99 -lm -g $(shell pkg-config allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)

objs = ballz.o logic.o

all = ballz 


ballz: $(objs)
	gcc $(objs) $(CFLAGS)  -o ballz 

ballz.o: ballz.c logic.h
	gcc $(CFLAGS) -c ballz.c 

logic.o: logic.c logic.h
	gcc $(CFLAGS) -c logic.c 

clean:
	rm -f *~ *.o

purge: clean
	rm -f ballz
