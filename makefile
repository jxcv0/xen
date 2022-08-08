CFILES = src/xen.c opengl/glad.c
FLAGS = -ggdb -O3 -Wall -Wextra -Werror
LIBS = -lm -lc -lglfw

build:
	gcc -o bin/xen -I src -I opengl $(CFILES) $(FLAGS) $(LIBS)
