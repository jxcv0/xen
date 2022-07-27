CFILES = src/xen.c src/camera.c src/graphics.c src/glad.c src/resources.c src/input.c src/window.c
FLAGS = -Wall -Werror -O3 -D XEN_DEBUG
LIBS = -ldl -lm -lc -lglfw -lpthread

build: mkbin $(CFILES)
	gcc -o bin/demo -I src demo/demo.c $(CFILES) $(FLAGS) $(LIBS)

clean:
	@rm -r bin/*
	
mkbin:
	@if [ ! -d "bin" ]; then mkdir bin; fi
