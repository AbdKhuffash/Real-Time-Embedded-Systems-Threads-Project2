LIBS = -lpthread -lGL -lGLU -lglut -lm -g
CC = gcc
OBJECTS = supermarket Customer Team openGL

all: $(OBJECTS)

%: %.c
	$(CC) $< -o $@ $(LIBS)

%.o: %.c
	$(CC) $< -c -o $@ $(LIBS)

run: supermarket
	./supermarket 

clean:
	rm -f *.o $(OBJECTS)
	./cleanMemory.sh
