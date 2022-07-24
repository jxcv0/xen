CFILES = src/xen.c src/graphics.c src/glad.c src/resources.c src/input.c src/window.c
FLAGS = -Wall -Werror -O3
LIBS = -ldl -lm -lc -lglfw -lpthread

build: mkbin $(CFILES)
	gcc -o bin/demo -I src demo/demo.c $(CFILES) $(FLAGS) $(LIBS)

clean:
	@rm -r bin/*
	
mkbin:
	@if [ ! -d "bin" ]; then mkdir bin; fi
